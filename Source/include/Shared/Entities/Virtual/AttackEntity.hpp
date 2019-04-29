#ifndef ATTACKENTITY_HPP
#define ATTACKENTITY_HPP

#include "Shared/Entities/Entity.hpp"

/**
 * Virtual parent class for attacks in the map
 *
 * \ingroup Entity
 * \ingroup Combat
 */
class AttackEntity : public Entity {
protected:
    Entity::Ptr attacker;

    /**
     * Creates the AttackEntity from the given attacker and animation file
     */
    AttackEntity(Entity::Ptr attacker, const std::string& anim);

public:
    /**
     * vtable
     */
    virtual ~AttackEntity() = default;

    /**
     * Creates either a MeleeAttackEntity or RangedAttackEntity based on what the CombatAttack is
     */
    static Entity::Ptr create(Entity::Ptr attacker, const CombatAttack& atk);

    /**
     * Pure virtual
     */
    const virtual std::string getType() = 0;

    /**
	 * Calls Entity::update()
	 */
	virtual void update();
};

#endif // RANGEDATTACK