#ifndef PARTICLEGENERATORENTITY_HPP
#define PARTICLEGENERATORENTITY_HPP

#include "Shared/Entities/Virtual/GraphicalEffectEntity.hpp"

//TODO - implement and integrate particle generators
enum ParticleGeneratorType {
    Fire,
    Ice,
    Poison
};

class ParticleGeneratorEntity : public GraphicalEffectEntity {
    ParticleGeneratorType type; //replace with actual generator

    /**
     * Creates the ParticleGeneratorEntity
     */
    ParticleGeneratorEntity(Entity::Ptr bindTo, EntityPosition pos, ParticleGeneratorType tp);

public:
    /**
     * Creates a ParticleGeneratorEntity and optionally binds it to the given Entity
     */
    static Entity::Ptr create(Entity::Ptr bindTo, EntityPosition pos, ParticleGeneratorType tp);

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
