#ifndef MAP_HPP
#define MAP_HPP

#include <string>
#include <vector>

#include "Shared/Util/Vector2d.hpp"
#include "Shared/Maps/Tileset.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Maps/Weather.hpp"
#include "Shared/Maps/MapTypes.hpp"

class Playlist;

class Map {
	std::string name, uniqueName;
	sf::Vector2i size;
	Tileset& tileset;
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

    int firstYSortLayer, firstTopLayer;
    int ambientLightOverride, currentLighting;

    static sf::RenderTexture lightTxtr;
    static sf::Sprite lightSpr;
    static sf::VertexArray light;

    int weatherType, weatherFreq;
    static Weather* weather; //TODO - singleton

	static bool staticMemebersCreated;

    Script::Ptr unloadScript;
    std::string unloadScriptStr, loadScriptStr;

    /**
     * Private constructor containing common init code
     */
	Map(Tileset& tlst);

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

    /**
     * Helper function for making MapScript objects
     */
    Script::Ptr makeMapScript(Script::Ptr scr, MapEvent evt, Entity::Ptr ent) {
        return Script::Ptr(new MapScript(*scr, name, evt.position, evt.size, ent, evt.runs));
    }

    static std::vector<std::string> visitedMaps;
    static std::map<std::string, std::vector<int> > pickedUpItems;

public:
	/**
	 * Creates the map with the given name and size. Editor only
	 */
	Map(std::string name, std::string uniqueName, sf::Vector2i size, Tileset& tileset, int nLayers = 5, int firstYSort = 2, int firstTop = 4);

	/**
     * Loads the map from the given file
     *
     * \param file The name of the map to load. Use "LastMap" to load the previous map
     * \param tileset A reference to the Tileset object
     * \param player A pointer to the player Entity object
     */
    Map(std::string file, Tileset& tileset, Entity::Ptr player = nullptr);

    /**
     * Runs the unload script, if any
     */
    ~Map();

    /**
     * Spawns the given Entity into the map using the given spawn
     */
    void spawnEntity(Entity::Ptr e, std::string spawn);

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
     * This function is used by an Entity when they move onto a tile.
     * It checks and plays animations that may have been activated while also checking scripts to run
     *
     * \param ent The Entity moving
     * \param oldBox The former bounding box
     */
    void moveOntoTile(Entity::Ptr ent, sf::FloatRect oldBox);

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
