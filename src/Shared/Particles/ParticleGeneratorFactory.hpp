#ifndef PARTICLEGENERATORFACTORY_HPP
#define PARTICLEGENERATORFACTORY_HPP

#include "Shared/Particles/ParticleGenerator.hpp"
#include <vector>
#include <string>

class ParticleGeneratorFactory {
public:
    enum Preset {
        None = 0,
        Smoke = 1,
        //TODO - other presets
    };

private:
    /**
     * Helper function to generate a smoke generator
     */
    static ParticleGenerator::Ptr createSmoke(ParticleGenerator::LifetimeType lifetimeType, float lifetime);

    /**
     * Helper function that returns a blank generator
     */
    static ParticleGenerator::Ptr createBlank();

public:
    /**
     * Creates a ParticleGenerator of the given type
     */
    static ParticleGenerator::Ptr create(Preset type, ParticleGenerator::LifetimeType lifetimeType, float lifetime);

    /**
     * Returns a readable list of all Presets
     */
    static const std::vector<std::string>& getAllPresets();

    /**
     * Returns the Preset with the given name
     */
    static Preset getPresetFromName(const std::string& name);
};

#endif // PARTICLEGENERATORFACTORY_HPP
