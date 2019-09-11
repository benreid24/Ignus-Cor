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

    EntityStats() : level(1), currentXp(0), health(100) {}
};

/**
 * Virtual base class for Entities that can fight
 *
 * \ingroup Entity
 * \ingroup Combat
 */
class CombatEntity : public Entity {
    float lastAttackTime;

protected:
    EntityStats stats;
    CombatAttack::ConstPtr weapon;
	CombatArmor::Ref armor;
	double xpRewardMultiplier; //xp reward = levelDiff^2 * xpRewardMultiplier + level

	/**
	 * Constructs the CombatEntity
	 */
    CombatEntity(std::string name, EntityPosition pos, std::string gfx1, std::string gfx2);

    /**
     * Destructor
     */
    virtual ~CombatEntity() = default;

    /**
     * Performs an attack using the current equipped CombatAttack
     *
     * \param atkDir Direction, in degrees, to attack in. Ranged only
     */
    void doAttack(int atkDir = 0);

    /**
     * Update logic to be applied before the time variable is updated
     */
    virtual void beforeTimerUpdate() override {}

    /**
     * Update logic to be applied after the timer variable is updated
     */
    virtual void afterTimerUpdate() override {}

    /**
	 * Notifies the Entity that they were attacked by another Entity
	 */
    void p_notifyAttacked(Ptr attacker, CombatAttack::ConstPtr attack) override final;

    /**
     * Notifies the Entity that they were attacked. For derived to override
     */
    virtual void p_notifyWasAttacked(Ptr attcker, CombatAttack::ConstPtr attack) {}

    /**
     * Notifies the Entity of nearby combat. For derived to override
     */
    virtual void p_notifyCombatNearby(List combatants) override {}

    /**
     * Notifies the Entity that they were interacted with. For derived to override
     */
    virtual void p_notifyInteracted(Ptr user) override {}

public:
    /**
	 * Pure virtual
	 */
	virtual const std::string getType() const = 0;

    /**
     * Returns the stats of the Entity
     */
    EntityStats& getStats();

    /**
     * Returns the current armor of the Entity
     */
    CombatArmor::Ref getArmor();
};

#endif // COMBATENTITY_HPP
