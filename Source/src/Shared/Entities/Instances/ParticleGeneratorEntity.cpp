#include "Shared/Entities/Instances/ParticleGeneratorEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Particles/ParticleGeneratorFactory.hpp"
using namespace std;

ParticleGeneratorEntity::ParticleGeneratorEntity(Entity::Ptr bindTo, EntityPosition pos, ParticleGenerator::Ptr gen,
                                                 const string& gfx, bool rm)
: GraphicalEffectEntity(bindTo, pos, gfx, rm) {
    generator = gen;
}

Entity::Ptr ParticleGeneratorEntity::create(Entity::Ptr bindTo, EntityPosition pos, ParticleGenerator::Ptr gen,
                                            const string& gfx, bool rm) {
    return Entity::Ptr(new ParticleGeneratorEntity(bindTo, pos, gen, gfx, rm));
}

void ParticleGeneratorEntity::render(sf::RenderTarget& target, sf::Vector2f camPos) {
    GraphicalEffectEntity::render(target, camPos);
    generator->render(target, position.coords-camPos);
}

void ParticleGeneratorEntity::update() {
    generator->update();
    if (boundTo.expired() && wasBound)
        generator->stop();
    if (generator->finished())
        EntityManager::get()->remove(this);
    GraphicalEffectEntity::update();
}

const string ParticleGeneratorEntity::getType() {
    return "ParticleGeneratorEntity";
}

bool ParticleGeneratorEntity::canDestroy() {
    return generator->finished();
}
