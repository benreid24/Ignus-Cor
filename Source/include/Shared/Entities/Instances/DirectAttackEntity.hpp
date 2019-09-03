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
    const std::string getType() const;

    /**
     * Update logic to be applied before the time variable is updated
     */
    void beforeTimerUpdate() override;

    /**
     * Update logic to be applied by derived classes after the timer is updated
     */
    void afterTimerUpdated() override;
};

#endif // MELEEATTACKENTITY_HPP
