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
        Left
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

	double lTime;
	float speed[2]; //speed in pixels/second for [slow,fast]. Will be set by child classes

	/**
	 * Helper function that notifies nearby Entities of being interacted with
	 */
	Ptr interact();

	static sf::Clock timer; //for doing movement based on time

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
	virtual const std::string getType() = 0;

	/**
	 * Updates the Entity. Base function just updates last update time
	 * Child overloads should call this when finished
	 */
	virtual void update();

	/**
	 * Notifies the Entity that they were attacked by another Entity
	 */
    virtual void notifyAttacked(Ptr attacker, CombatAttack::ConstPtr attack);

    /**
     * Notifies the Entity of nearby combat
     */
    virtual void notifyCombatNearby(List combatants);

    /**
     * Notifies the Entity that they were interacted with
     */
    virtual void notifyInteracted(Ptr user);

    /**
	 * Returns the bounding box for the Entity
	 */
	sf::FloatRect getBoundingBox();

	/**
	 * Returns the bounding box of the interaction zone
	 */
    sf::FloatRect getInteractBox();

    /**
     * Returns the coordinates of the center of the Entity
     */
    sf::Vector2f getCenter();

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
	std::string getName();

	/**
	 * Generates a string to represent the Entity
	 */
    std::string getIdString();

	/**
	 * Returns the position of the Entity
	 */
	EntityPosition getPosition();

	/**
	 * Renders the Entity to the given target
	 */
	virtual void render(sf::RenderTarget& target, sf::Vector2f camPos);

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
    bool collidesWithOtherEntities();

    /**
     * Returns whether or not the Entity is y-sorted when rendering, or if it should render on top of other Entities
     */
    bool isYSortRendered();
};

#endif // ENTITY_HPP
