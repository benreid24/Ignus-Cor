#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Shared/Particles/ParticleGraphics.hpp"

/**
 * Base class to represent a Particle from a ParticleGenerator. Implements basic behaviors
 * but can be sub-classed for more complex ones
 *
 * \ingroup Particles
 */
class Particle {
public:
    /**
     * Represents the 'lifetime' of the Particle. i.e. when it should be destroyed
     * For value based lifetimes like time and distance there is a fuzziness applied
     */
    enum LifetimeType {
        GeneratorLifetime, ///Exists until the owning ParticleGenerator is destroyed
        DistanceTraveledLifetime, ///Exists until it travels a certain distance
        TimeAliveLifetime, ///Exists until it is alive for a certain time
        AnimationDoneLifetime ///Exists until the Animation is finished
    };

    /**
     * Represents how the velocity, angle, and opacity of the Particle change
     */
    enum ChangeType {
        NoChange,
        DistanceTraveledChange,
        TimeAliveChange
    };

    /**
     * Represents how the change to velocity, angle, and opacity is applied
     */
    enum ChangeScaleType {
        Constant, //m
        Linear, // m*x
        Exponential, // m*x^2
        Decay, // m/x
        ExponentialDecay // m/x^2
    };

    /**
     * Storage container to define behaviors for velocity, rotation, and opacity
     */
    struct Behavior {
        ChangeType changeType = Particle::NoChange;
        float multiplier = 0;
        ChangeScaleType scaleType;

        /**
         * Computes the new value based on the rate of change and time elapsed
         */
        float getChange(float timeElapsed, float distanceTraveled);
    };

protected:
    ParticleGraphics graphics;
    sf::Vector2f position;
    float direction, velocity, opacity;

    sf::Vector2f spawnPosition;
    float lastUpdateTime;
    float spawnDir, spawnVel, spawnOpacity, spawnTime;

    Behavior rotationBehavior, velocityBehavior, opacityBehavior;

    LifetimeType lifetimeType;
    float lifetimeValue;

    /**
     * Helper function to get distance traveled
     */
    float getSquaredDistance();

public:
    /**
     * Creates the Particle from it's graphics, position, direction, and velocity
     */
    Particle(const ParticleGraphics& gfx, sf::Vector2f pos, float dir, float vel, float opacity, float currentTime);

    /**
     * vtable
     */
    virtual ~Particle() = default;

    /**
     * Adds a basic behavior to the Particle's rotation
     */
    void setRotationBehavior(Behavior behavior);

    /**
     * Adds a basic behavior to the Particle's velocity
     */
    void setVelocityBehavior(Behavior behavior);

    /**
     * Adds a basic behavior to the Particle's opacity
     */
    void setOpacityBehavior(Behavior behavior);

    /**
     * Adds a lifetime to the Particle
     */
    void setLifetime(LifetimeType lifetime, float value);

    /**
     * Returns whether or not the Particle should be destroyed
     */
    bool isFinished(float currentTime);

    /**
     * Returns whether or not this Particle existing should delay the destruction of the ParticleGenerator
     */
    bool blocksGeneratorDestruction();

    /**
     * Updates the Particle, applying any behavior set on it
     */
    virtual void update(float currentTime);

    /**
     * Renders to the target relative to the position of the generator
     */
    virtual void render(sf::RenderTarget& target, sf::Vector2f generatorPos);
};

#endif // PARTICLE_HPP
