#ifndef PARTICLEGENERATOR_HPP
#define PARTICLEGENERATOR_HPP

#include "Shared/Particles/Particle.hpp"
#include <list>
#include <cmath>

/**
 * \defgroup Particles All classes and functions related to Particles and ParticleGenerators
 */

 /**
  * Base ParticleGenerator. Can leverage the built in behaviors in Particle to make a generic
  * generator, or can be sub-classed for more advanced functionality
  *
  * \ingroup Particles
  */
class ParticleGenerator {
public:
    /**
     * Defines the different lifetime options for the generator
     */
    enum LifetimeType {
        UntilDestroyedLifetime, ///Stays functional until explicitly destroyed
        TimeExistedLifetime, ///Stays alive for a given amount of time
        ParticlesGeneratedLifetime, ///Generates a fixed amount of Particles then is done
    };

    /**
     * Represents how the generation rate changes over time
     */
    enum GenerationRateChangeScale {
        Linear,
        Exponential,
        Decay,
        ExponentialDecay
    };

    /**
     * Storage container for a window of values
     */
    struct ValueWindow {
        float minimum, maximum;

        ValueWindow(float v1, float v2) : minimum(std::min(v1,v2)), maximum(std::max(v1,v2)) {}

        float getValue();
    };

protected:
    sf::Clock timer;
    std::list<Particle*> particles;

    ParticleGraphics defaultParticleGraphics;
    Particle::Behavior rotationBehavior, velocityBehavior, opacityBehavior;
    Particle::LifetimeType particleLifetimeType;
    float particleLifetimeValue, particleLifetimeFuzziness;

    ValueWindow spawnRadius, spawnAngle;
    ValueWindow spawnVelocity, spawnDirection;

    LifetimeType lifetimeType;
    float lifetimeValue;

    float particlesPerSecond;
    GenerationRateChangeScale genChgType;
    float genChgMultiplier;

public:
    /**
     * Constructs the base ParticleGenerator
     */
    ParticleGenerator(ValueWindow spwnRadius, ValueWindow spwnAngle, ValueWindow spwnVel, ValueWindow spwnDir);

    /**
     * Frees all memory
     */
    virtual ~ParticleGenerator();

    /**
     * Sets the default Particle behaviors
     */
    void setParticleBehaviors(Particle::Behavior rotBehave, Particle::Behavior velBehave, Particle::Behavior opacBehave);

    /**
     * Sets the default Particle lifetime
     */
    void setParticleLifetime(Particle::LifetimeType lifetimeType, float lifetimeValue, float fuzziness);

    /**
     * Sets the default ParticleGraphics
     */
    void setParticleGraphics(const std::string& gfx);

    /**
     * Sets the generator's own lifetime
     */
    void setLifetime(LifetimeType lifetimeType, float lifetime);

    /**
     * Sets the generation rate and behavior
     */
    void setGenerationRate(float rate, GenerationRateChangeScale rateChgType, float rateChgMultiplier);

    /**
     * Begins termination of the generator
     */
    void stop();

    /**
     * Returns whether or not the generator is finished, i.e. all finite lifetime Particles are destroyed
     */
    bool finished();

    /**
     * Updates the ParticleGenerator
     */
    virtual void update();

    /**
     * Renders the particles
     */
    virtual void render(sf::RenderTarget& target, sf::Vector2f pos);

};

#endif // PARTICLEGENERATOR_HPP
