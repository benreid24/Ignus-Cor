#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include "Shared/Entities/Entity.hpp"

class EntityManager {
	std::vector<Entity::Ptr> entities;
    std::vector<std::vector<Entity::Ptr> > ySortedEntities;
	Entity* player;

	/**
	 * Internal function for Entities to update their y-sort if they move
	 */
	void updatePosition(Entity::Ptr e, float lastY, float curY);

	friend class Entity;

public:
	/**
	 * Creates an EntityManager
	 */
	EntityManager();

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
	void setMapHeight(int height);

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
	 * Returns a reference to the y-sorted list of Entities for the map to render
	 */
	std::vector<std::vector<Entity::Ptr> >& getYSorted();
};

#endif // ENTITYMANAGER_HPP
