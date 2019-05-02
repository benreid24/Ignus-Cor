#ifndef MELEEATTACKENTITY_HPP
#define MELEEATTACKENTITY_HPP

#include "Shared/Entities/Virtual/AttackEntity.hpp"

/**
 * Entity that is created with melee attacks
 *
 * \ingroup Entity
 * \ingroup Combat
 */
class MeleeAttackEntity : public AttackEntity {
    CombatAttack attack;
    sf::Vector2f offset;
    bool follow;

    /**
     * Creates the MeleeAttackEntity instance from the attacking Entity and attack
     */
    MeleeAttackEntity(Entity::Ptr attacker, const CombatAttack& attack, bool followAttacker = true);

public:
    /**
     * Creates a Ptr to a MeleeAttackEntity
     */
    static Entity::Ptr create(Entity::Ptr attacker, const CombatAttack& attack, bool followAttacker = true);

    /**
     * vtable
     */
    virtual ~MeleeAttackEntity() = default;

    /**
     * Returns "MeleeAttackEntity"
     */
    const std::string getType();

    /**
     * Updates the attack and damages Entities that get hit
     */
    void update();
};

#endif // MELEEATTACKENTITY_HPP
