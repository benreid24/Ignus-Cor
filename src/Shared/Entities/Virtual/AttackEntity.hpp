#ifndef ATTACKENTITY_HPP
#define ATTACKENTITY_HPP

#include "Shared/Entities/Entity.hpp"
#include "Shared/Combat/CombatAttack.hpp"

/**
 * Virtual parent class for attacks in the map
 *
 * \ingroup Entity
 * \ingroup Combat
 */
class AttackEntity : public Entity {
    Entity::List entitiesHit;
    bool particlesCreated;

protected:
    Entity::Ptr attacker;
    CombatAttack::Ref attack;

    /**
     * Helper function to keep track of who has been hit already
     */
    bool shouldApplyDamage(Entity::Ptr ent);

    /**
     * Creates the AttackEntity from the given attacker and animation file
     */
    AttackEntity(Entity::Ptr attacker, CombatAttack::Ref atk);

    /**
     * Update logic to be applied before the time variable is updated
     */
    virtual void beforeTimerUpdate() override {}

    /**
     * Update logic to be applied after the timer variable is updated
     */
    virtual void afterTimerUpdate() override final;

    /**
     * Update logic to be applied by derived classes after the timer is updated
     */
    virtual void p_afterTimerUpdate() {}

public:
    /**
     * vtable
     */
    virtual ~AttackEntity() = default;

    /**
     * Creates either a DirectAttackEntity or RangedAttackEntity based on what the CombatAttack is
     *
     * \param attacker The Entity doing the attack
     * \param atk The attack being used
     * \param atkDir Direction to attack in, in degrees. Ranged only
     */
    static Entity::Ptr create(Entity::Ptr attacker, CombatAttack::Ref atk, int atkDir);

    /**
     * Pure virtual
     */
    const virtual std::string getType() const override = 0;
};

#endif // RANGEDATTACK
