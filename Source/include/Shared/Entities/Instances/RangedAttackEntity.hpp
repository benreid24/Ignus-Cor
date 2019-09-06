#ifndef RANGEDATTACKENTITY_HPP
#define RANGEDATTACKENTITY_HPP

#include "Shared/Entities/Virtual/AttackEntity.hpp"

/**
 * Entity for objects created from ranged attacks (arrows, spells, etc). Generally move in one
 * direction until hitting an Entity and applying damage
 *
 * \ingroup Entity
 * \ingroup Combat
 */
class RangedAttackEntity : public AttackEntity {
    float cosDir, sinDir, distanceTraveled;

    /**
     * Creates the RangedAttackEntity from the given attacker and attack
     */
    RangedAttackEntity(Entity::Ptr attacker, CombatAttack::ConstPtr attack, int atkDir);

    /**
     * Update logic to be applied before the time variable is updated
     */
    virtual void beforeTimerUpdate() override;

public:
    /**
     * Creates the projectile from the weapon/spell and source Entity
     */
    static Entity::Ptr create(Entity::Ptr attacker, CombatAttack::ConstPtr attack, int atkDir);

    /**
     * vtable
     */
    virtual ~RangedAttackEntity() = default;

    /**
     * Returns "RangedAttackEntity"
     */
    virtual const std::string getType() const override;
};

#endif // RANGEDATTACK


