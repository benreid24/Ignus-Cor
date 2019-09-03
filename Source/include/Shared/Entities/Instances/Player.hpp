#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Shared/Entities/Virtual/CombatEntity.hpp"

/**
 * Class to represent the player(s) in the game. Stores inventory and other player data
 */
class Player : public CombatEntity {
	//player data (money, inventory, etc)

	/**
	 * Initializes the player object with all of the default values
	 */
	Player();

	/**
	 * Helper function to get attack direction based on mouse position
	 */
    int getAttackDirection() const;

public:
	typedef std::shared_ptr<Player> Ptr;

	/**
	 * Destructor
	 */
    virtual ~Player() = default;

	/**
	 * Creates a Player and returns a pointer
	 */
	static Ptr create();

	/**
     * Update logic to be applied before the time variable is updated
     */
    void beforeTimerUpdate() override final;

    /**
     * Update logic to be applied after the timer variable is updated
     */
    void afterTimerUpdate() override final {}

	/**
	 * Returns "Player"
	 */
	const std::string getType() const;
};

#endif // PLAYER_HPP
