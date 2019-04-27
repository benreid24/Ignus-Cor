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
     * Creates the AttackEntity from the given attacker and weapon
     */
    AttackEntity(Entity::Ptr attacker);

public:
    /**
     * vtable
     */
    virtual ~AttackEntity() = default;

    /**
     * Returns "AttackEntity"
     */
    const std::string getType();

    /**
	 * Pure virtual
	 */
	virtual void update() = 0;
};

#endif // RANGEDATTACK
