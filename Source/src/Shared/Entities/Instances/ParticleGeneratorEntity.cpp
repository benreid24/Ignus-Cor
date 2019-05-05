#include "Shared/Entities/Instances/ParticleGeneratorEntity.hpp"
using namespace std;

ParticleGeneratorEntity::ParticleGeneratorEntity(Entity::Ptr bindTo, EntityPosition pos, ParticleGenerator::Ptr gen, const string& gfx)
: GraphicalEffectEntity(bindTo, pos, gfx) {
    generator = gen;
}

Entity::Ptr ParticleGeneratorEntity::create(Entity::Ptr bindTo, EntityPosition pos, ParticleGenerator::Ptr gen, const string& gfx) {
    return Entity::Ptr(new ParticleGeneratorEntity(bindTo, pos, gen, gfx));
}

void ParticleGeneratorEntity::render(sf::RenderTarget& target, sf::Vector2f camPos) {
    GraphicalEffectEntity::render(target, camPos);
    generator->render(target, position.coords-camPos);
}

void ParticleGeneratorEntity::update() {
    generator->update();
    if (generator->finished())
        EntityManager::get()->remove(this);
    GraphicalEffectEntity::update();
}

const string ParticleGeneratorEntity::getType() {
    return "ParticleGeneratorEntity";
}
