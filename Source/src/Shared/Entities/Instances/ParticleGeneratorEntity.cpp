#include "Shared/Entities/Instances/ParticleGeneratorEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
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
    if (generator->finished())
        EntityManager::get()->remove(this);
    GraphicalEffectEntity::update();
}

const string ParticleGeneratorEntity::getType() {
    return "ParticleGeneratorEntity";
}

Entity::Ptr ParticleGeneratorEntity::createSmoke(Entity::Ptr bindTo, EntityPosition pos, float lifetime) {
    ParticleGenerator::ValueWindow spRad(0,7), spAng(0,360), spVel(2,5), spDir(-85,-110);
    ParticleGenerator::Ptr gen = ParticleGenerator::create(spRad, spAng, spVel, spDir);
    gen->setGenerationRate(20, ParticleGenerator::Linear, -20/lifetime);
    gen->setLifetime(ParticleGenerator::UntilDestroyedLifetime, lifetime);

    Particle::Behavior rotBehavior, velBehavior, opBehavior;
    rotBehavior.changeType = Particle::DistanceTraveledChange;
    rotBehavior.scaleType = Particle::Linear;
    rotBehavior.multiplier = -15/20;
    velBehavior.scaleType = Particle::Constant;
    velBehavior.multiplier = 0.1;
    opBehavior.changeType = Particle::DistanceTraveledChange;
    opBehavior.scaleType = Particle::Exponential;
    opBehavior.multiplier = -0.05;
    gen->setParticleBehaviors(rotBehavior, velBehavior, opBehavior);
    gen->setParticleLifetime(Particle::DistanceTraveledLifetime, 65, 10);
    gen->setParticleGraphics("smoke.png");

    return Entity::Ptr(new ParticleGeneratorEntity(bindTo, pos, gen, "", false));
}
