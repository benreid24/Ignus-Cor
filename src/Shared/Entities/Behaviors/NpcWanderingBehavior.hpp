#ifndef NPCWANDERINGBEHAVIOR_HPP
#define NPCWANDERINGBEHAVIOR_HPP

#include "Shared/Entities/Behaviors/EntityBehavior.hpp"

/**
 * Basic wandering behavior for Entities. Simply wander around in a given radius
 *
 * \ingroup Entity
 */
class NpcWanderingBehavior : public EntityBehavior { //TODO - make another layer of behavior classes. Npc base, enemy base, creature base, etc
    int radius;
    double sqrdRad;
    EntityPosition::Direction movementDirection;
    unsigned long timeOfNextStep, lastStepTime;

    static constexpr int Standing = 50; //this is how sub-behaviors can define custom states

    /**
     * Helper function to determine direction to walk in
     */
    EntityPosition::Direction getMovementDirection();

    /**
     * Helper function that returns the square magnitude of the given vector
     *
     * \param v The vector whose magnitude should be calculated
     * \return The magnitude of the vector squared
     */
    int magnitudeSquared(sf::Vector2i v);

    /**
     * Updates the owning Entity
     */
    virtual void p_update() override;

    /**
     * Enters interaction state
     */
    virtual void p_notifyInteracted(Entity::Ptr interactor) override;

public:
    /**
     * Creates the NpcWanderingBehavior with the given owner and radius
     */
    NpcWanderingBehavior(Entity* owner, int radius = 600);
};

#endif // NPCWANDERINGBEHAVIOR_HPP
