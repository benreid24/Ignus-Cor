#ifndef COMBATENTITY_HPP
#define COMBATENTITY_HPP

#include "Shared/Entities/Entity.hpp"
#include "Shared/Combat/CombatArmor.hpp"

/**
 * Stores combat related stats for an Entity
 *
 * \ingroup Entity
 */
struct EntityStats {
    double health, maxHealth;
    int level;
    double currentXp, nextLevelXp;

    EntityStats() : level(1), currentXp(0) {}
};

/**
 * Virtual base class for Entities that can fight
 *
 * \ingroup Entity
 * \ingroup Combat
 */
class CombatEntity : public Entity {
    long int lastAttackTime;

protected:
    EntityStats stats;
    CombatAttack weapon;
	CombatArmor armor;
	double xpRewardMultiplier; //xp reward = levelDiff^2 * xpRewardMultiplier + level

	/**
	 * Constructs the CombatEntity
	 */
    CombatEntity(std::string name, EntityPosition pos, std::string gfx1, std::string gfx2);

    /**
     * Performs an attack using the current equipped CombatAttack
     */
    void doAttack();

public:
    /**
	 * Pure virtual
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
    virtual void notifyAttacked(Ptr attacker, const CombatAttack& attack);

    /**
     * Notifies the Entity of nearby combat
     */
    virtual void notifyCombatNearby(List combatants);

    /**
     * Notifies the Entity that they were interacted with
     */
    virtual void notifyInteracted(Ptr user);

    /**
     * Returns the stats of the Entity
     */
    EntityStats& getStats();

    /**
     * Returns the current armor of the Entity
     */
    const CombatArmor& getArmor();
};

#endif // COMBATENTITY_HPP
