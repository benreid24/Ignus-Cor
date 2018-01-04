#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../Entity.hpp"

/**
 * Class to represent the player(s) in the game. Stores inventory and other player data
 */
class Player : public Entity {
	//player data (money, inventory, etc)

	/**
	 * Initializes the player object with all of the default values
	 */
	Player();

public:
	typedef std::shared_ptr<Player> Ptr;

	/**
	 * Creates a Player and returns a pointer
	 */
	static Ptr create();

	/**
	 * Listens for player input and moves/acts accordingly
	 */
	virtual void update();

	/**
	 * Returns "Player"
	 */
	const std::string getType();
};

#endif // PLAYER_HPP
