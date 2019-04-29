#ifndef WANDERINGBEHAVIOR_HPP
#define WANDERINGBEHAVIOR_HPP

#include "Shared/Entities/EntityBehavior.hpp"

/**
 * Basic wandering behavior for Entities. Simply wander around in a given radius
 *
 * \ingroup Entity
 */
class WanderingBehavior : public EntityBehavior {
    int radius;
    double sqrdRad;
    EntityPosition::Direction movementDirection;

    sf::Clock timer;
    int timeOfNextStep;

    static const int Standing = 50; //this is how sub-behaviors can define custom states

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

public:
    /**
     * Creates the WanderingBehavior with the given owner and radius
     */
    WanderingBehavior(Entity* owner, int radius = 600);

    /**
     * Updates the owning Entity
     */
    virtual void update();
};

#endif // WANDERINGBEHAVIOR_HPP
