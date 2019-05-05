#ifndef PARTICLEGENERATORENTITY_HPP
#define PARTICLEGENERATORENTITY_HPP

#include "Shared/Entities/Virtual/GraphicalEffectEntity.hpp"
#include "Shared/Particles/ParticleGenerator.hpp"

class ParticleGeneratorEntity : public GraphicalEffectEntity {
    ParticleGenerator::Ptr generator;

    /**
     * Creates the ParticleGeneratorEntity
     */
    ParticleGeneratorEntity(Entity::Ptr bindTo, EntityPosition pos, ParticleGenerator::Ptr gen, const std::string& gfx, bool removeWithBound = false);

public:
    /**
     * Creates a ParticleGeneratorEntity and optionally binds it to the given Entity
     */
    static Entity::Ptr create(Entity::Ptr bindTo, EntityPosition pos, ParticleGenerator::Ptr gen, const std::string& gfx, bool removeWithBound = false);

    /**
     * Convenience function to create a generator for smoke
     */
    static Entity::Ptr createSmoke(Entity::Ptr bindTo, EntityPosition pos, float lifetime);

    /**
     * vtable
     */
    virtual ~ParticleGeneratorEntity() = default;

    /**
     * Returns "ParticleGeneratorEntity"
     */
    const std::string getType();

    /**
     * Renders the particles
     */
    virtual void render(sf::RenderTarget& target, sf::Vector2f camPos);

    /**
     * Updates the generator
     */
    virtual void update();
};

#endif // PARTICLEGENERATORENTITY_HPP
