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

void ParticleGeneratorEntity::p_renderAbove(sf::RenderTarget& target, sf::Vector2f camPos) {
    generator->render(target, position.coords, camPos);
}

void ParticleGeneratorEntity::beforeTimerUpdate() {
    generator->update();
    if (boundTo.expired() && wasBound)
        generator->stop();
    if (generator->finished())
        EntityManager::get()->remove(this);
}

const string ParticleGeneratorEntity::getType() const {
    return "ParticleGeneratorEntity";
}

bool ParticleGeneratorEntity::canDestroy() {
    return generator->finished();
}
