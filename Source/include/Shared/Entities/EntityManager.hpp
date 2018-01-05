#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include "Shared/Entities/Entity.hpp"

class Game;

/**
 * Stores and manages all of the active Entities across all maps
 *
 * \ingroup Entity
 */
class EntityManager {
	std::vector<Entity::Ptr> entities;
    std::map<std::string, std::vector<std::vector<Entity::Ptr> > > ySortedEntities;
    Game* game;
	Entity* player;

	/**
	 * Internal function for Entities to update their y-sort if they move
	 */
	void updatePosition(Entity* e, EntityPosition oldPos);

	/**
	 * Helper function to determine if an Entity is occupying a space
	 */
	bool spaceFree(Entity* e, EntityPosition space, sf::Vector2f size);

	/**
	 * Internal function for Entities to check if movement is valid
	 */
	bool canMove(Entity* e, EntityPosition oldPos, EntityPosition newPos, sf::Vector2f size);

	friend class Entity;

public:
	/**
	 * Creates an EntityManager
	 */
	EntityManager(Game* g);

	/**
	 * Deletes all Entities excluding the player
	 */
	~EntityManager();

	/**
	 * Deletes all of the Entities that are owned
	 */
	void clear();

	/**
	 * Sets the size of the map for the purpose of y-sorting. This MUST be called before adding Entities
	 */
	void registerMap(std::string mapname, int height);

	/**
	 * Adds the given Entity
	 */
	void add(Entity::Ptr e);

	/**
	 * Removes the given Entity by name, and optionally by type
	 */
	void remove(std::string name, std::string type = "");

	/**
	 * Removes the given Entity based on memory address
	 */
	void remove(Entity::Ptr e);

	/**
	 * Updates all Entities
	 */
	void update();

	/**
	 * Returns a reference to the y-sorted list of Entities for the given map to render
	 */
	std::vector<std::vector<Entity::Ptr> >& getYSorted(std::string mapname);

	/**
	 * Proxy function for the Player Entity to update the render position through the MapManager
	 */
	void updateRenderPosition(sf::Vector2f playerCoords);
};

#endif // ENTITYMANAGER_HPP
