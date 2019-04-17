#ifndef MAPMANAGER_HPP
#define MAPMANAGER_HPP

#include "Map.hpp"
#include "Tileset.hpp"
#include <map>

class Game;
class EntityManager;
class Entity;
class SoundEngine;
class Playlist;

/**
 * Class to store all loaded maps and handle updating and rendering
 *
 * \ingroup World
 */
class MapManager {
    /**
     * Helper struct for storing loaded maps
     */
    struct MapHolder {
        std::shared_ptr<Map> mapdata;
        int lastActiveTime;
    };

    std::map<std::string, MapHolder> maps;
    Tileset tileset;
    std::string activeMap;
    sf::Clock timer;

    Entity::Ptr player;
    EntityManager* entityManager;
    Playlist* playlist;
    SoundEngine* soundEngine;

public:
    /**
     * Defaults internal data and loads tileset
     */
    MapManager(EntityManager* em, SoundEngine* se, Playlist* plst);

    /**
     * Sets the internal reference to the player
     */
	void setPlayer(Entity::Ptr player);

    /**
     * Loads the requested map if it is not already available
     */
	void loadMap(std::string mapFile);

    /**
     * Moves the given Entity to the requested map
     */
    void mapChange(Entity* ent, std::string mapFile, std::string spawnName);

    /**
     * Updates the camera position in the active map
     */
	void updateRenderPosition(sf::Vector2f playerCoords);

	/**
	 * Checks if a given movement is valid for the given map
	 */
	bool movementValid(EntityPosition oldPos, EntityPosition newPos, sf::Vector2f size);

	/**
	 * Checks if the given space is free
	 */
	bool spaceFree(EntityPosition space, sf::Vector2f size);

    /**
     * Updates all internal maps and removes stale maps from memory
     */
    void update();

    /**
     * Renders the currently active map
     */
    void render();
};

#endif // MAPMANAGER_HPP
