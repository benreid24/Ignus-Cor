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

    Game* game;
    Entity* player;
    EntityManager* entityManager;
    Playlist* playlist;
    SoundEngine* soundEngine;

public:
    /**
     * Defaults internal data and loads tileset
     */
    MapManager();

    /**
     * Sets the internal pointers to other game classes
     */
    void setPointers(Game* g, Entity* player, EntityManager* em, SoundEngine* se, Playlist* plst);

    /**
     * Moves the given Entity to the requested map
     */
    void mapChange(Entity* ent, std::string mapFile, std::string spawnName);

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
