#ifndef PARTICLEGENERATORFACTORY_HPP
#define PARTICLEGENERATORFACTORY_HPP

#include "Shared/Particles/ParticleGenerator.hpp"

class ParticleGeneratorFactory {
public:
    enum Preset {
        Smoke = 0,
        //TODO - other presets
    };

private:
    /**
     * Helper function to generate a smoke generator
     */
    static ParticleGenerator::Ptr createSmoke(float lifetime);

    /**
     * Helper function that returns a blank generator
     */
    static ParticleGenerator::Ptr createBlank();

public:
    /**
     * Creates a ParticleGenerator of the given type
     */
    static ParticleGenerator::Ptr create(Preset type, ParticleGenerator::LifetimeType lifetimeType, float lifetime);
};

#endif // PARTICLEGENERATORFACTORY_HPP
