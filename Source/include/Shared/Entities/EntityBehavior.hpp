#ifndef ENTITYBEHAVIOR_HPP
#define ENTITYBEHAVIOR_HPP

#include <memory>
#include <list>
#include "Shared/Entities/Entity.hpp"

// Potential candidates:
//     - Patrol
//     - Wandering
//     - DayNightCycler
//     - Pack
//     - More

/**
 * Base class for all Entity behaviors
 *
 * \ingroup Entity
 */
class EntityBehavior {
public:
    /**
     * How the Entity should respond to being attacked or witnessing combat
     */
    enum CombatTendency {
        Inanimate = -1,
        Normal = 0,
        Timid = 1,
        Aggressive,
        Hostile,
        Ally
    }combatTendency;

    enum State {
        Default = 0,
        Fleeing,
        Fighting,
        Aiding,
        Interacting,
        Custom = 50 //values above 50 are guaranteed to be free. Use for custom state in child classes
    };

private:
    std::list<Entity::WeakPtr> attackers; //TODO - allies?
    Entity::WeakPtr interactor;

protected:
    Entity::WeakPtr owner;

    /**
     * Accessor function for Entity interactor. Returns nullptr if none or expired
     */
    Entity::Ptr getInteractor();

    /**
     * Accessor function for attackers
     */
    std::list<Entity::Ptr> getAttackers();


public:
    /**
     * Construct the behavior with an owner
     */
    EntityBehavior(Entity::Ptr owner);

    /**
     * For vtable
     */
    virtual ~EntityBehavior() = default;

    /**
     * Perform one update cycle. This will potentially modify the owning Entity
     */
    virtual void update() = 0;

    /**
     * Notify of being attacked. Child classes should call this to update the attacker list
     */
    virtual void notifyAttacked(Entity::Ptr attacker, const CombatAttack& attack);

    /**
     * Notify of nearby combat
     */
     virtual void notifyCombatNearby(Entity::List combatants);

    /**
     * Notify of being interacted with. Child classes should call this to update the interactor WeakPtr
     */
    virtual void notifyInteracted(Entity::Ptr interactor);
};

#endif // BEHAVIOR_HPP
