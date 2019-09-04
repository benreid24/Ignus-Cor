#ifndef MELEEATTACKENTITY_HPP
#define MELEEATTACKENTITY_HPP

#include "Shared/Entities/Virtual/AttackEntity.hpp"

/**
 * Entity that is created with melee attacks
 *
 * \ingroup Entity
 * \ingroup Combat
 */
class DirectAttackEntity : public AttackEntity {
    sf::Vector2f offset;
    bool follow;

    /**
     * Creates the DirectAttackEntity instance from the attacking Entity and attack
     */
    DirectAttackEntity(Entity::Ptr attacker, CombatAttack::ConstPtr attack, bool followAttacker = true);

    /**
     * Update logic to be applied before the time variable is updated
     */
    virtual void beforeTimerUpdate() override;

    /**
     * Update logic to be applied by derived classes after the timer is updated
     */
    virtual void p_afterTimerUpdate() override;

public:
    /**
     * Creates a Ptr to a DirectAttackEntity
     */
    static Entity::Ptr create(Entity::Ptr attacker, CombatAttack::ConstPtr attack, bool followAttacker = true);

    /**
     * vtable
     */
    virtual ~DirectAttackEntity() = default;

    /**
     * Returns "DirectAttackEntity"
     */
    virtual const std::string getType() const override;
};

#endif // MELEEATTACKENTITY_HPP
