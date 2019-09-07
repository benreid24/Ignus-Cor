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

    struct StateValue {
        int lastState;

        StateValue() { state = lastState = EntityBehavior::Default; }
        StateValue(const int s) { state = lastState = s; }
        StateValue& operator=(const int s) { lastState = state; state = s; return *this; }
        operator int() { return state; }

    private:
        int state;
    }state;

    /**
     * For derived classes to implement custom update behavior
     */
    virtual void p_update() = 0;

    /**
     * Notify derived class of being attacked
     */
    virtual void p_notifyAttacked(Entity::Ptr attacker, CombatAttack::ConstPtr attack) {}

    /**
     * Notify derived class of nearby combat
     */
    virtual void p_notifyCombatNearby(Entity::List combatants) {}

    /**
     * Notify derived class of being interacted with
     */
    virtual void p_notifyInteracted(Entity::Ptr interactor) {}

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
     void update();

    /**
     * Notify of being attacked
     */
    void notifyAttacked(Entity::Ptr attacker, CombatAttack::ConstPtr attack);

    /**
     * Notify of nearby combat
     */
    void notifyCombatNearby(Entity::List combatants);

    /**
     * Notify of being interacted with
     */
    void notifyInteracted(Entity::Ptr interactor);

    /**
     * Terminates the interaction
     */
    void terminateInteraction();

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
