#include "Shared/Entities/Instances/ParticleGeneratorEntity.hpp"
using namespace std;

ParticleGeneratorEntity::ParticleGeneratorEntity(Entity::Ptr bindTo, EntityPosition pos, ParticleGeneratorType tp)
: GraphicalEffectEntity(bindTo, pos, "") {
    //init generator
}

Entity::Ptr ParticleGeneratorEntity::create(Entity::Ptr bindTo, EntityPosition pos, ParticleGeneratorType tp) {
    return Entity::Ptr(new ParticleGeneratorEntity(bindTo, pos, tp));
}

void ParticleGeneratorEntity::render(sf::RenderTarget& target, sf::Vector2f camPos) {
    //render
}

void ParticleGeneratorEntity::update() {
    //update
    GraphicalEffectEntity::update();
}

const string ParticleGeneratorEntity::getType() {
    return "ParticleGeneratorEntity";
}
