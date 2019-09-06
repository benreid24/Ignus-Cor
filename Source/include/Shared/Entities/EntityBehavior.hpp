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

    /**
     * Helper function to flee from enemies
     */
    void doFlee();

    /**
     * Helper function to fight enemies
     */
    void doFight();

    /**
     * Helper function to do interaction
     */
    void doInteract();

protected:
    EntityPosition startPosition;
    Entity* owner;
    int state;

public:
    /**
     * Construct the behavior with an owner
     */
    EntityBehavior(Entity* owner);

    /**
     * For vtable
     */
    virtual ~EntityBehavior() = default;

    /**
     * Perform one update cycle. This will potentially modify the owning Entity
     */
    virtual void update();

    /**
     * Notify of being attacked. Child classes should call this to update the attacker list
     */
    virtual void notifyAttacked(Entity::Ptr attacker, CombatAttack::ConstPtr attack);

    /**
     * Notify of nearby combat
     */
    virtual void notifyCombatNearby(Entity::List combatants);

    /**
     * Notify of being interacted with. Child classes should call this to update the interactor WeakPtr
     */
    virtual void notifyInteracted(Entity::Ptr interactor);

    /**
     * Accessor function for Entity interactor. Returns nullptr if none or expired
     */
    Entity::Ptr getInteractor();

    /**
     * Accessor function for attackers
     */
    Entity::List getAttackers();
};

#endif // BEHAVIOR_HPP
