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
    virtual void beforeBehaviorUpdate() {}

    /**
     * Update logic to be applied after the Behavior
     * Applied before the timer is updated
     */
    virtual void afterBehaviorUpdate() {}

    /**
     * Update logic to be applied after the timer variable is updated
     */
    virtual void afterTimerUpdate() override {}

    /**
	 * Notifies the Entity that they were attacked by another Entity
	 */
    void p_notifyWasAttacked(Ptr attacker, CombatAttack::Ref attack) override final;

    /**
     * Notifies the Entity of nearby combat
     */
    void p_notifyCombatNearby(List combatants) override final;

    /**
     * Notifies the Entity that they were interacted with
     */
    void p_notifyInteracted(Ptr user) override final;

    /**
	 * Notifies the Entity that they were attacked by another Entity. For derived to override
	 */
    virtual void p_notifyAiAttacked(Ptr attacker, CombatAttack::Ref attack) {}

    /**
     * Notifies the Entity of nearby combat. For derived to override
     */
    virtual void p_notifyAiCombatNearby(List combatants) {}

    /**
     * Notifies the Entity that they were interacted with. For derived to override
     */
    virtual void p_notifyAiInteracted(Ptr user) {}

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
};

#endif // AIENTITY_HPP
