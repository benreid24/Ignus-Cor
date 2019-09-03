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

    /**
     * Returns generator->finished()
     */
    bool canDestroy();

public:
    /**
     * Creates a ParticleGeneratorEntity and optionally binds it to the given Entity
     */
    static Entity::Ptr create(Entity::Ptr bindTo, EntityPosition pos, ParticleGenerator::Ptr gen, const std::string& gfx = "", bool removeWithBound = false);

    /**
     * vtable
     */
    virtual ~ParticleGeneratorEntity() = default;

    /**
     * Returns "ParticleGeneratorEntity"
     */
    const std::string getType() const;

    /**
     * Allows derived classes to provide custom rendering below the graphics
     */
    virtual void customRenderBelow(sf::RenderTarget& target, sf::Vector2f camPos) {}

    /**
     * Allows derived classes to provide custom rendering above the graphics
     */
    virtual void customRenderAbove(sf::RenderTarget& target, sf::Vector2f camPos);

    /**
     * Update logic to be applied before the time variable is updated
     */
    virtual void beforeTimerUpdate() override;
};

#endif // PARTICLEGENERATORENTITY_HPP
