#ifndef MAP_HPP
#define MAP_HPP

#include <string>
#include <vector>
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Util/ResourcePool.hpp"
#include "Shared/Util/Vector2d.hpp"
#include "Shared/Scripts/Script Interpreter.hpp"
#include "Shared/Maps/Tileset.hpp"
#include "Shared/Util/File.hpp"

/**
 * Structure to store a tile in the map
 *
 * \ingroup World
 */
struct Tile
{
    /**
     * Creates an empty tile
     */
    Tile()
    {
        nonZero = false;
        isAnim = false;
    }
    bool isAnim, nonZero, delA;
    Animation* anim;
    sf::Sprite spr;
};

/**
 * Structure to store a light in the map
 *
 * \ingroup World
 */
struct Light
{
    sf::Vector2f position;
    int radius;
};

/**
 * Structure to store a script in the map
 *
 * \ingroup World
 */
struct MapEvent
{
    ScriptReference script;
    std::string scriptStr;
    sf::Vector2i position;
    sf::Vector2i size;
    int maxRuns, trigger; //0 = on load, 1 = step in range, 2 = step out of range, 3 = step in or out of range, 4 = in range
    int runs;
};

class Map {
	std::string name;
	sf::Vector2i size;
	Tileset& tileset;

	std::vector<Animation> anims;
    std::vector<Vector2D<Tile> > layers;
    std::vector<Vector2D<std::pair<int,Tile*> > > ySortedTiles; //here for actual image size
    Vector2D<int> collisions, catchables, charCols; //using int b/c stupid specialization of vector<bool> conflicting with 2d vector

    std::vector<Light> lights;
    std::vector<MapEvent> events;
    sf::Vector2f camPos;
    sf::Vector2i camPosTiles;

    std::string lastMap, pcMap, curMap;
    sf::Vector2f lastPos;
    int lastDir, pcSpawn;

    int firstYSortLayer, firstTopLayer;
    int ambientLightOverride, currentLighting;
    sf::RenderTexture lightTxtr;
    sf::Sprite lightSpr;
    sf::VertexArray light;

    //Weather weather; TODO - copy in weather
    ScriptReference unloadScript;

    /**
     * Deletes the entire map from memory
     */
    void clear();

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
	Map(std::string name, sf::Vector2i size, Tileset& tileset);

	/**
	 * Loads the map from the given file
	 */
	Map(std::string file, Tileset& tileset);

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
     * Loads the map from the given file
     *
     * \param file The name of the map to load. Use "LastMap" to load the previous map
     * \param spId The id of the spawn to put the player at. Use 0 to put the player where they were in the previous map
     * \param trans Whether or not to do a smooth transition
     */
    void load(std::string file = "", int spId = 0, bool trans = true); //TODO - Add pointer to player here for spawning

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
     */
	void editTile(int x, int y, int layer, int nId);

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
