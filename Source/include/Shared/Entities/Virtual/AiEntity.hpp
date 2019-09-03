#ifndef AIENTITY_HPP
#define AIENTITY_HPP

#include "Shared/Entities/Virtual/CombatEntity.hpp"
#include "Shared/Entities/EntityConversation.hpp"

class EntityBehavior;

/**
 * Parent class for AI controlled Entities
 *
 * \ingroup Entity
 */
class AiEntity : public CombatEntity {
protected:
    EntityBehavior* behavior;
    EntityConversation conversation;

    /**
     * Update logic to be applied before the time variable is updated
     */
    void beforeTimerUpdate() override final;

    /**
     * Update logic to be applied before the Behavior
     * Applied before the time is updated
     */
    virtual void beforeBehaviorUpdate() = 0;

    /**
     * Update logic to be applied after the Behavior
     * Applied before the timer is updated
     */
    virtual void afterBehaviorUpdate() = 0;

    /**
     * Update logic to be applied after the timer variable is updated
     */
    virtual void afterTimerUpdate() override = 0;

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
    virtual const std::string getType() const = 0;

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
