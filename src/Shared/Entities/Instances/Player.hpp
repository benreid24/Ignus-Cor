#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Shared/Entities/Virtual/CombatEntity.hpp"

/**
 * \defgroup Player Classes related to the human player
 */

/**
 * Class to represent the player(s) in the game. Stores inventory and other player data
 *
 * \ingroup Entities
 * \ingroup Player
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

    /**
     * Update logic to be applied before the time variable is updated
     */
    virtual void beforeTimerUpdate() override final;

	/**
	 * Returns "Player"
	 */
	virtual const std::string getType() const override;

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
};

#endif // PLAYER_HPP
