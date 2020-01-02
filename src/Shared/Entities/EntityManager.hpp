#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include "Shared/Entities/Entity.hpp"
#include <unordered_map>

class Game;

/**
 * Stores and manages all of the active Entities across all maps
 *
 * \ingroup Entity
 */
class EntityManager {
    sf::Mutex entityLock;
    Entity::List entityDeleteQueue, entityAddQueue;

	std::vector<Entity::Ptr> entities;
	std::unordered_map<Entity*, Entity::WeakPtr> entityPointerMap;
    std::map<std::string, std::vector<std::vector<Entity::Ptr> > > ySortedEntities;
    std::map<std::string, std::vector<Entity::Ptr> > topEntities;
	Entity::Ptr player;

	/**
	 * Internal function for Entities to update their y-sort if they move
	 */
	void updatePosition(Entity* e, EntityPosition oldPos);

	/**
	 * Helper function to determine if an Entity is occupying a space
	 */
	bool spaceFree(Entity* e, EntityPosition space, sf::Vector2f size);

    /**
     * Performs the actual deletion of an Entity
     */
    void doDelete(Entity::Ptr e);

    /**
     * Performs the actual addition of an Entity
     */
    void doAdd(Entity::Ptr e);

    /**
	 * Creates an EntityManager
	 */
	EntityManager();

	/**
	 * Deletes all Entities excluding the player
	 */
	~EntityManager();

	friend class Entity;

public:
	static EntityManager& get();

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
     * Returns an Entity::Ptr from an Entity*
     */
    Entity::Ptr getEntityPtr(Entity* ent); //TODO - replace with std::enable_shared_from_this

	/**
	 * Removes the given Entity by name, and optionally by type
	 */
	void remove(std::string name, std::string type = "");

	/**
	 * Removes the given Entity based on Entity::Ptr
	 */
	void remove(Entity::Ptr e);

	/**
	 * Removes the given Entity using a raw pointer
	 */
    void remove(Entity* e);

	/**
	 * Updates all Entities
	 */
	void update();

	/**
	 * Check if movement is valid
	 */
	bool canMove(Entity* e, EntityPosition oldPos, EntityPosition newPos, sf::Vector2f size);

	/**
	 * Returns a reference to the y-sorted list of Entities for the given map to render
	 */
	std::vector<std::vector<Entity::Ptr> >& getYSortedEntities(std::string mapname);

	/**
	 * Returns a reference to the list of Entities to render on top of the given map
	 */
    std::vector<Entity::Ptr>& getTopEntities(std::string mapname);

	/**
	 * Returns an Entity::List with all the Entities in the given space
	 */
    Entity::List getEntitiesInSpace(const std::string& mapName, const sf::FloatRect& area, bool respectCollisions = true);
};

#endif // ENTITYMANAGER_HPP
