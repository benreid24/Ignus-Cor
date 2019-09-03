#ifndef CREATUREENTITY_HPP
#define CREATUREENTITY_HPP

#include "Shared/Entities/Virtual/AiEntity.hpp"
#include "Shared/Util/File.hpp"

/**
 * Entity class for Creatures, ie Entities that should not persist. Typically created from spawners
 *
 * \ingroup Entity
 */
class CreatureEntity : public AiEntity {
    /**
     * Creates the CreatureEntity from the given data and data file
     */
    CreatureEntity(std::string nm, EntityPosition pos, std::string gfx1, std::string gfx2, File& dataFile);

public:
    /**
     * Loads the Creature from the data file and returns a Ptr to the created object
     */
    static Entity::Ptr create(std::string dataFile); //TODO - create from spawner

    /**
     * vtable
     */
    virtual ~CreatureEntity() = default;

    /**
     * Returns "CreatureEntity"
     */
    const std::string getType() const;

    /**
     * Update logic to be applied before the Behavior
     * Applied before the time is updated
     */
    virtual void beforeBehaviorUpdate() override {}

    /**
     * Update logic to be applied after the Behavior
     * Applied before the timer is updated
     */
    virtual void afterBehaviorUpdate() override {}

    /**
     * Update logic to be applied after the timer variable is updated
     */
    virtual void afterTimerUpdate() override {}

	/**
	 * Notifies the Entity that they were attacked by another Entity. For derived to override
	 */
    virtual void notifyAiAttacked(Ptr attacker, CombatAttack::ConstPtr attack) override {}

    /**
     * Notifies the Entity of nearby combat. For derived to override
     */
    virtual void notifyAiCombatNearby(List combatants) override {}

    /**
     * Notifies the Entity that they were interacted with. For derived to override
     */
    virtual void notifyAiInteracted(Ptr user) override {}
};

#endif // CREATUREENTITY_HPP

