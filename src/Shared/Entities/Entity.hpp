#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <memory>
#include <list>
#include "Shared/Entities/EntityVisual.hpp"
#include "Shared/Entities/EntityBubble.hpp"
#include "Shared/Combat/CombatAttack.hpp"

/**
 * \defgroup Entity
 * \brief All of the "physical" objects in the game and classes to manage them
 */

class EntityBehavior;
class EntityManager;

/**
 * Stores the position of an Entity. Position consists of coordinates and map
 */
struct EntityPosition {
	sf::Vector2f coords;
	std::string mapName;

	enum Direction {
        Up = 0,
        Right,
        Down,
        Left,
		Any
	}dir;

	/**
	 * Zeros the position
	 */
	EntityPosition() : coords(0,0) { dir = Up; }

	/**
	 * Assigns the given values to the position
	 */
	EntityPosition(sf::Vector2f pos, std::string map, Direction d) {
		coords = pos;
		mapName = map;
		dir = d;
	}

	/**
	 * Assigns the given values to the position. Gets the map from the MapManager
	 */
	EntityPosition(sf::Vector2f pos, Direction d);
};

/**
 * Base class for all "physical" objects in the game. Provides an interface for the game to manage it
 *
 * \ingroup Entity
 */
class Entity {
public:
	typedef std::shared_ptr<Entity> Ptr;
	typedef std::weak_ptr<Entity> WeakPtr;
	typedef std::list<Ptr> List;

protected:
	int uuid; //TODO - figure out best way to persist Entities across maps/gamesaves
	std::string name;

	EntityPosition position;
	EntityVisual graphics;
	EntityBubble bubble;

	sf::FloatRect boundingBox; //relative to upper left corner (position and gfx origin)
	float interactDistance; //how far away it can interact
	bool collisionsEnabled; //if it blocks other Entities
	bool isYSorted; //true to render in y sort, false to render on top

	float speed[2]; //speed in pixels/second for [slow,fast]. Will be set by child classes

	/**
	 * Helper function that notifies nearby Entities of being interacted with
	 */
	Ptr interact();

	/**
     * Update logic to be applied before the time variable is updated
     */
    virtual void beforeTimerUpdate() {}

    /**
     * Update logic to be applied after the timer variable is updated
     */
    virtual void afterTimerUpdate() {}

    /**
     * Allows derived classes to provide custom rendering below the graphics
     */
    virtual void p_renderBelow(sf::RenderTarget& target, sf::Vector2f camPos) {}

    /**
     * Allows derived classes to provide custom rendering above the graphics
     */
    virtual void p_renderAbove(sf::RenderTarget& target, sf::Vector2f camPos) {}

    /**
	 * Notifies the Entity that they were attacked by another Entity. For derived to override
	 */
    virtual void p_notifyAttacked(Ptr attacker, CombatAttack::Ref attack) {}

    /**
     * Notifies the Entity of nearby combat. For derived to override
     */
    virtual void p_notifyCombatNearby(List combatants) {}

    /**
     * Notifies the Entity that they were interacted with. For derived to override
     */
    virtual void p_notifyInteracted(Ptr user) {}

public:
	/**
	 * Creates the Entity with the given name, position, and graphics
	 */
	Entity(std::string nm, EntityPosition pos, std::string gfx1, std::string gfx2 = "");

	/**
	 * Virtual destructor for magic
	 */
	virtual ~Entity();

	/**
	 * Returns the type of the Entity. To be defined by children
	 */
	virtual const std::string getType() const = 0;

	/**
	 * Updates the Entity. Derived classes should overload beforeTimerUpdate() and afterTimerUpdate()
	 * to provide custom functionality
	 */
	void update();

	/**
	 * Notifies the Entity that they were attacked by another Entity
	 */
    void notifyAttacked(Ptr attacker, CombatAttack::Ref attack);

    /**
     * Notifies the Entity of nearby combat
     */
    void notifyCombatNearby(List combatants);

    /**
     * Notifies the Entity that they were interacted with
     */
    void notifyInteracted(Ptr user);

    /**
	 * Returns the bounding box for the Entity
	 */
	sf::FloatRect getBoundingBox() const;

	/**
	 * Returns the bounding box of the interaction zone
	 */
    sf::FloatRect getInteractBox() const;

    /**
     * Returns the coordinates of the center of the Entity
     */
    sf::Vector2f getCenter() const;

	/**
	 * Sets the position and direction of the Entity. Leave direction empty to maintain it. Position is in pixels
	 */
	void setPositionAndDirection(EntityPosition pos);

	/**
	 * Shifts the Entity by the given amount of pixels. Also optionally changes the map
	 */
	void shift(sf::Vector2f amount, const std::string& newMap = "");

	/**
	 * Returns the name of the Entity
	 */
	const std::string& getName() const;

	/**
	 * Generates a string to represent the Entity
	 */
    std::string getIdString() const;

	/**
	 * Returns the position of the Entity
	 */
	EntityPosition getPosition() const;

	/**
	 * Returns a reference to the EntityBubble
	 */
    EntityBubble& getBubble();

	/**
	 * Renders the Entity to the given target
	 */
	void render(sf::RenderTarget& target, sf::Vector2f camPos);

	/**
	 * Moves the Entity in the given direction. Actual amount is based on if fast and how much time elapsed
	 *
	 * \param dir The direction to move in
	 * \param fast Whether or not to move fast
	 * \param elapsedTime Elapsed time to use, leave blank for the function to figure it out
	 * \return True if the Entity was able to move, false otherwise
	 */
	bool move(EntityPosition::Direction dir, bool fast = false, float elapsedTime = 0);

	/**
	 * Returns whether or not this Entity should be considered when doing collisions between other Entities
	 */
    bool collidesWithOtherEntities() const;

    /**
     * Returns whether or not the Entity is y-sorted when rendering, or if it should render on top of other Entities
     */
    bool isYSortRendered() const;

    /**
     * Returns the elapsed time between calls to update()
     */
    double timeSinceLastUpdate() const;

private:
    double timeLastUpdated;
};

#endif // ENTITY_HPP
