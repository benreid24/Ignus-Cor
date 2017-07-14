#ifndef ENTITY_HPP
#define ENTITY_HPP

/**
 * Base class for all "physical" objects in the game. Provides an interface for the game to manage it
 */
class Entity {
	int uuid;
	std::string name;
	sf::Vector2f position;

public:
	/**
	 * Creates the Entity with the given name
	 */
	Entity(std::string nm);
};

#endif // ENTITY_HPP
