#ifndef RANGEDATTACKENTITY_HPP
#define RANGEDATTACKENTITY_HPP

#include "Shared/Entities/Virtual/AttackEntity.hpp"
#include "Shared/Combat/CombatAttack.hpp"

/**
 * Entity for objects created from ranged attacks (arrows, spells, etc). Generally move in one
 * direction until hitting an Entity and applying damage
 *
 * \ingroup Entity
 * \ingroup Combat
 */
class RangedAttackEntity : public AttackEntity {
    CombatAttack attack;

    /**
     * Creates the RangedAttackEntity from the given attacker and attack
     */
    RangedAttackEntity(Entity::Ptr attacker, const CombatAttack& attack);

public:
    /**
     * Creates the projectile from the weapon/spell and source Entity
     */
    static Entity::Ptr create(Entity::Ptr attacker, const CombatAttack& attack);

    /**
     * vtable
     */
    virtual ~RangedAttackEntity() = default;

    /**
     * Returns "RangedAttackEntity"
     */
    const std::string getType();

    /**
	 * Updates the Entity. Moves in same direction. Applies damage and deletes if it hits another Entity
	 */
	void update();
};

#endif // RANGEDATTACK


