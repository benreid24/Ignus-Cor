#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include "Shared/Entities/EntityVisual.hpp"
#include "Shared/Entities/EntityBubble.hpp"

/**
 * \defgroup Entity
 * \brief All of the "physical" objects in the game and classes to manage them
 */

/**
 * Base class for all "physical" objects in the game. Provides an interface for the game to manage it
 *
 * \ingroup Entity
 */
class Entity {
	int uuid;
	std::string name;
	sf::Vector2f position;
	EntityVisual graphics;
	EntityBubble bubble;
	int dir, lTime;

	static sf::Clock timer; //for doing movement based on time

public:
	/**
	 * Creates the Entity with the given name, position, and graphics
	 */
	Entity(std::string nm, sf::Vector2f pos, std::string gfx1, std::string gfx2 = "");

	/**
	 * Returns the type of the Entity. To be defined by children
	 */
	virtual const std::string getType() = 0;

	/**
	 * Returns the position of the Entity
	 */
	sf::Vector2f getPosition();

	/**
	 * Sets the position and direction of the Entity. Leave direction empty to maintain it
	 */
	void setPositionAndDirection(sf::Vector2f pos, int d = -1);

	/**
	 * Shifts the Entity by the given amount of pixels. Set truncate to true to force the Entity onto a tile border
	 */
	void shift(sf::Vector2f amount, bool truncate = false);

	/**
	 * Returns the direction the Entity is facing
	 */
    int getDir();

	/**
	 * Returns the name of the Entity
	 */
	std::string getName();

	/**
	 * Renders the Entity to the given target
	 */
	void render(sf::RenderTarget& target);

	/**
	 * Moves the Entity in the given direction. Actual amount is based on if fast and how much time elapsed
	 */
	void move(int dir, bool fast = false);
};

#endif // ENTITY_HPP
