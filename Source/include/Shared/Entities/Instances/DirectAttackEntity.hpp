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
    DirectAttackEntity(Entity::Ptr attacker, const CombatAttack& attack, bool followAttacker = true);

public:
    /**
     * Creates a Ptr to a DirectAttackEntity
     */
    static Entity::Ptr create(Entity::Ptr attacker, const CombatAttack& attack, bool followAttacker = true);

    /**
     * vtable
     */
    virtual ~DirectAttackEntity() = default;

    /**
     * Returns "DirectAttackEntity"
     */
    const std::string getType();

    /**
     * Updates the attack and damages Entities that get hit
     */
    void update();
};

#endif // MELEEATTACKENTITY_HPP
