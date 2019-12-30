#ifndef MAPTYPES_HPP
#define MAPTYPES_HPP

#include "Shared/Media/Animation.hpp"
#include "Shared/Maps/MapEvent.hpp"

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
        isAnim = false;
    }
    bool isAnim;
    std::shared_ptr<Animation> anim;
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

#endif