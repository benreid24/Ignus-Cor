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

class Playlist;

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
    int radius;
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
 * Structure to store details of a spawner that is in a map
 *
 * \ingroup Map
 */
struct MapSpawner {
	sf::Vector2f position;
	int coolDown, frequency;
	int numActiveLimit, spawnLimit;
	std::string templateFile;
	//TODO - add actual spawner data or make a separate class
};

class Map {
	std::string name;
	sf::Vector2i size;
	Tileset& tileset;
	EntityManager* entityManager;
	std::string music;

	std::map<int,Animation*> animTable;
    std::vector<Vector2D<Tile> > layers;
    std::vector<Vector2D<std::pair<int,Tile*> > > ySortedTiles; //here for actual image size
    Vector2D<int> collisions;

    std::vector<Light> lights;
    std::vector<MapEvent> events;
    sf::Vector2f camPos;
    sf::Vector2i camPosTiles;

    static std::string lastMap, curMap;
    static sf::Vector2f lastPos;
    static int lastDir;

    int firstYSortLayer, firstTopLayer;
    int ambientLightOverride, currentLighting;

    static sf::RenderTexture lightTxtr;
    static sf::Sprite lightSpr;
    static sf::VertexArray light;

    static ScriptEnvironment* scriptEnv;
    static Weather* weather;
	static bool staticMemebersCreated;

    ScriptReference unloadScript;
    std::string unloadScriptStr;

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
    Map(std::string file, Tileset& tileset, EntityManager* entityManager, SoundEngine* se, Entity* player = nullptr, std::string spName = "", Playlist* plst = nullptr);

    /**
     * Runs the unload script, if any
     */
    ~Map();

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
     * Adds a new layer at the specified position
     */
	void addLayer(int i = -1);

	/**
	 * Deletes the given layer
	 */
	void removeLayer(int i);

	/**
	 * Sets the first y-sort layer
	 */
	void setFirstYSortLayer(int i);

	/**
	 * Returns the first y-sort layer
	 */
	int getFirstYSortLayer();

	/**
	 * Sets the first top layer
	 */
	void setFirstTopLayer(int i);

	/**
	 * Returns the first top layer
	 */
	int getFirstTopLayer();

	/**
	 * Returns a reference to the filename of the playlist to use
	 */
	std::string& getMusic();

    /**
     * Sets the current weather. See Weather::Type for legal values to pass
     *
     * \param t The type of weather
     */
    void setWeather(int t);

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
     */
    void draw(sf::RenderTarget& target, std::vector<int> layerList);

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
     * Modifies the world collision map
     *
     * \param x The x coordinate of the collision to change
     * \param y The y coordinate of the collision to change
     * \param c The type of collision to set there (0 = none, 1 = all, 2 = top, 3 = right, 4 = bottom, 5 = left, 6 = topRight, 7 = bottomRight, 8 = bottomLeft, 9 = topLeft, 10 = topBottom, 11 = rightLeft, 12 = noTop, 13 = noRight, 14 = noBottom, 15 = noLeft)
     */
	void setCollision(int x, int y, int c);

	/**
	 * Returns the collision at the specified position
	 */
	int getCollision(int x, int y);

    /**
     * Returns whether or not the given space can be moved into
     *
     * \param position The space to check
     * \param oldPos The old (current) position of the entity trying to move
     * \return Whether or not the space can be moved into
     */
    bool spaceFree(sf::Vector2i position, sf::Vector2i oldPos);

    /**
     * Special version of spaceFree for PathFinder. Only returns true for spaces that can be moved onto from all edges
     *
     * \param position The space to check
     * \return Whether or not the space can be moved into
     */
    bool spaceFree(sf::Vector2i position);

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
     * Adds a light to the world
     *
     * \param x The x position of the light in world pixels
     * \param y The y position of the light in world pixels
     * \param r The radius of the light in pixels
     */
    void addLight(int x, int y, int r);

    /**
     * Removes the light at the given position from the world
     *
     * \param x The x position of the light to remove
     * \param y The y position of the light to remove
     */
    void removeLight(int x, int y);

    /**
     * Removes all lights from the world
     */
    void removeAllLights();

    /**
     * Sets the ambient lighting to the given value, rather than letting it be determined by the time of day. Use when inside
     *
     * \param o The light level [0,255]. Use 255 to let time control the ambient light level
     */
    void setLightingOverride(int o);

    /**
     * Edits the tile at the given position to the new id
     *
     * \param x The x position of the tile
     * \param y The y position of the tile
     * \param layer The layer of the tile
     * \param nId The new id to set the tile to
     * \param isAnim Whether or not the new tile should be an animation or image
     */
	void editTile(int x, int y, int layer, int nId, bool isAnim);

	/**
	 * Syncs up the internal animation table with the tileset. Use this when adding/removing tiles from the tileset
	 */
	void syncAnimTable();

	/**
	 * Removes all references to tiles/animations that no longer exist in the tileset. Call this after deleting a tile/animation
	 */
	void clearBrokenTiles();

	/**
	 * Adds the given MapEvent to the map
	 */
	void addEvent(MapEvent event);

	/**
	 * Removes the Event at the given position
	 */
	void removeEvent(int x, int y);

	/**
	 * Returns a pointer to the MapEvent at the given position, or nullptr if none found
	 */
	MapEvent* getEvent(int x, int y);

	//TODO - functions for ai and items and other things
};

#endif // MAP_HPP
