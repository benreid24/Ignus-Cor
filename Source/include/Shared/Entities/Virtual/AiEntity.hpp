#ifndef AIENTITY_HPP
#define AIENTITY_HPP

#include "Shared/Entities/Virtual/CombatEntity.hpp"

class EntityBehavior;

/**
 * Parent class for AI controlled Entities
 *
 * \ingroup Entity
 */
class AiEntity : public CombatEntity {
protected:
    EntityBehavior* behavior;

public:
    /**
     * Basic constructor
     */
    AiEntity(std::string nm, EntityPosition pos, std::string gfx1, std::string gfx2 = "");

    /**
     * vtable
     */
    virtual ~AiEntity();

    /**
     * Pure virtual
     */
    virtual const std::string getType() = 0;

    /**
	 * Updates the Entity. Just calls behavior if present
	 */
	virtual void update();

	/**
	 * Notifies the Entity that they were attacked by another Entity
	 */
    virtual void notifyAttacked(Ptr attacker, CombatAttack::ConstPtr attack);

    /**
     * Notifies the Entity of nearby combat
     */
    virtual void notifyCombatNearby(List combatants);

    /**
     * Notifies the Entity that they were interacted with
     */
    virtual void notifyInteracted(Ptr user);
};

#endif // AIENTITY_HPP
