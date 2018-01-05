#ifndef MAP_HPP
#define MAP_HPP

#include <string>
#include <vector>
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Util/ResourcePool.hpp"
#include "Shared/Util/Vector2d.hpp"
#include "Shared/Scripts/Script Environment.hpp"
#include "Shared/Maps/Tileset.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Maps/Weather.hpp"
#include "Shared/Entities/Instances/ItemEntity.hpp"

class Playlist;
class Map;

/**
 * Structure to store a tile in the map
 *
 * \ingroup World
 */
struct Tile {
    /**
     * Creates an empty tile
     */
    Tile() {
        nonZero = false;
        isAnim = false;
        delA = false;
    }
    bool isAnim, nonZero, delA;
    Animation* anim;
    sf::Sprite spr;
    int id;
};

/**
 * Structure to store a light in the map
 *
 * \ingroup World
 */
struct Light {
    sf::Vector2f position;
    int radius, threshold;
};

/**
 * Structure to store a script in the map
 *
 * \ingroup World
 */
struct MapEvent {
    ScriptReference script;
    std::string scriptStr;
    sf::Vector2i position;
    sf::Vector2i size;
    int maxRuns, trigger; //0 = on load, 1 = step in range, 2 = step out of range, 3 = step in or out of range, 4 = in range
    int runs;
};

/**
 * Struct to store entity spawn data
 */
struct EntitySpawn {
	std::string name;
	EntityPosition position;
};

/**
 * Structure to store details of a spawner that is in a map
 *
 * \ingroup Map
 */
struct MapSpawner {
	sf::Vector2i position;
	int coolDown, frequency;
	int numActiveLimit, spawnLimit;
	std::string templateFile;
	//TODO - add actual spawner data or make a separate class
};

/**
 * Structure to store details of items in the map
 *
 * \ingroup Map
 */
struct MapItem {
	int itemId, mapId;
	sf::Vector2i position;

private:
	Entity::Ptr ie;
	friend class Map;
};

class Map {
	std::string name, uniqueName;
	sf::Vector2i size;
	Tileset& tileset;
	EntityManager* entityManager;
	std::string music;

	std::map<int,Animation*> animTable;
    std::vector<Vector2D<Tile> > layers;
    std::vector<Vector2D<std::pair<int,Tile*> > > ySortedTiles; //here for actual image size

    Vector2D<int> collisions;
	static std::vector<TextureReference> collisionTextures;
	static sf::Sprite collisionSprite;
	TextureReference arrowTexture, spawnerTexture;
	sf::Sprite arrowSprite, spawnerSprite;
	sf::Text utilText;

	std::vector<MapSpawner> spawners;
    std::vector<Light> lights;
    std::vector<MapEvent> events;
    sf::Vector2f camPos;
    sf::Vector2i camPosTiles;

    std::vector<MapItem> items;
    int maxMapItemId;

    std::vector<EntitySpawn> entitySpawns;
    static std::string lastMap, curMap;
    static EntityPosition lastPos; //TODO - With many maps implementation, does this have to be static? Each map could store it's own and it would persist

    int firstYSortLayer, firstTopLayer;
    int ambientLightOverride, currentLighting;

    static sf::RenderTexture lightTxtr;
    static sf::Sprite lightSpr;
    static sf::VertexArray light;

    int weatherType, weatherFreq;
    static Weather* weather;

	static bool staticMemebersCreated;

	static ScriptEnvironment* scriptEnv;
    ScriptReference unloadScript;
    std::string unloadScriptStr, loadScriptStr;

    /**
     * Private constructor containing common init code
     */
	Map(Tileset& tlst, SoundEngine* se);

	/**
	 * Resets all of the y-sorted tiles
	 */
	void resetYSorted();

    /**
     * Adds the given map to the list of visited maps
     *
     * \param m The name of the place visited
     */
    static void addVisitedMap(std::string m);

    static std::vector<std::string> visitedMaps;
    static std::map<std::string, std::vector<int> > pickedUpItems;

public:
	/**
	 * Creates the map with the given name and size
	 */
	Map(std::string name, sf::Vector2i size, Tileset& tileset, SoundEngine* se, EntityManager* entityManager, int nLayers = 5, int firstYSort = 2, int firstTop = 4);

	/**
     * Loads the map from the given file
     *
     * \param file The name of the map to load. Use "LastMap" to load the previous map
     * \param tileset A reference to the Tileset object
     * \param entityManager A pointer to the EntityManager
     * \param se A pointer to the SoundEngine object
     * \param player A pointer to the player Entity object
     * \param spName The name of the spawn to put the player at. Use "prev" to put the player where they were in the previous map
     * \param plst A pointer to the Playlist object to update, if any
     */
    Map(std::string file, Tileset& tileset, EntityManager* entityManager, SoundEngine* se, Entity* player = nullptr, Playlist* plst = nullptr);

    /**
     * Runs the unload script, if any
     */
    ~Map();

    /**
     * Spawns the given Entity into the map using the given spawn
     */
    void spawnEntity(Entity* e, std::string spawn);

    /**
     * Sets the environment for map scripts to run in
     */
    static void setScriptEnvironment(ScriptEnvironment* se);

	/**
     * Returns the name of the currently loaded map
     */
	std::string getName();

	/**
	 * Returns the size of the currently loaded map in tiles
	 */
	sf::Vector2i getSize();

    /**
     * Saves all the world data to the given file. This is for game saves
     *
     * \param saveTo The file to save to
     */
    static void saveGame(File* saveTo);

    /**
     * Loads all the world data from the given file. This is for game saves
     *
     * \param loadFrom The file to load from
     */
    static void loadGame(File* loadFrom);

    /**
     * Saves the map to the given file
     */
    void save(std::string file);

    /**
     * Sets the current weather. See Weather::Type for legal values to pass
     *
     * \param t The type of weather
     */
    void setWeather(int t);

    /**
     * Returns the current weather type
     */
	int getWeatherType();

    /**
     * Tells whether or not the given map has been visited
     *
     * \param m The name of the map to check
     * \return Whether or not the map has been visited
     */
    static bool mapVisited(std::string m);

    /**
     * Updates the world and all of the entities within it
     */
    void update();

    /**
     * Calculates the new ambient light level, taking time of day, overrides and weather into account
     */
	void calculateLighting();

    /**
     * Renders the world to the given target
     *
     * \param target The target to render to
     */
    void draw(sf::RenderTarget& target);

    /**
     * Renders the world to the given target using a filter for layers
     *
     * \param target The target to render to
     * \param layerList The layers to draw
     * \param selection The selection. Set x = -1 for no selection
     * \param showEvents Whether or not to show events
     */
    void draw(sf::RenderTarget& target, std::vector<int> layerList, sf::IntRect selection, bool showEvents);

    /**
     * Sets the camera position based on the players position while making sure that nothing off-map gets rendered
     *
     * \param playerPos The position of the player in world pixels
     */
    void setRenderPosition(sf::Vector2f playerPos); //called by player class each time it moves

    /**
     * This function is used by the player when they move onto a tile. It calls the other moveOnToTile function, while also checking scripts to run and deciding whether or not the player should be attacked
     *
     * \param playerPos The current tile position of the player
     * \param lastPos The last position the player was in
     */
    void moveOntoTile(sf::Vector2i playerPos, sf::Vector2i lastPos);

    /**
     * Checks and plays animations that may have been activated. This function is used by all entities that move
     *
     * \param pos The position of the entity
     */
    void moveOntoTile(sf::Vector2i pos); //version used by other characters and scripts

    /**
     * Returns the position of the camera in world pixels
     */
    sf::Vector2f getCamera();

    /**
     * Returns whether or not the given space can be moved into
     *
     * \param oldBox The old (current) bounding box of the entity trying to move
     * \param newBox The bounding box to check in
     * \return Whether or not the space can be moved into
     */
    bool spaceFree(sf::FloatRect oldBox, sf::FloatRect newBox);

    /**
     * Special version of spaceFree for PathFinder. Only returns true for spaces that can be moved onto from all edges
     *
     * \param position The space to check
     * \param box the bounding box to check in
     * \return Whether or not the space can be moved into
     */
    bool spaceFree(sf::FloatRect box);

    /**
     * Sets the given item to a status where it won't be loaded again when the map is loaded
     *
     * \param id The map unique id of the item
     */
    void setItemPickedUp(int id);

    /**
     * Locks all of the entities in the world. Used when the game is paused
     */
    void lockAllPeople();

    /**
     * Unlocks all of the entities in the world, used when the game is unpaused
     */
    void unlockAllPeople();

    /**
     * Stops all movement animations in Character's
     */
	void stopAnimations();

	/**
	 * Returns all tiles occupied by a bounding box
	 */
	static std::vector<sf::Vector2i> getCoveredTiles(sf::FloatRect box);

	#include "Map_priv.hpp"

	//TODO - functions for ai and items and other things
};

#endif // MAP_HPP
