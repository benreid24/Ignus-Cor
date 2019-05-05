#include "Shared/Particles/ParticleGeneratorFactory.hpp"
using namespace std;

ParticleGenerator::Ptr ParticleGeneratorFactory::createSmoke(float lifetime) {
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

    return gen;
}

ParticleGenerator::Ptr ParticleGeneratorFactory::createBlank() {
    ParticleGenerator::ValueWindow spRad(0,7), spAng(0,360), spVel(2,5), spDir(-85,-110);
    ParticleGenerator::Ptr gen = ParticleGenerator::create(spRad, spAng, spVel, spDir);
    gen->setGenerationRate(0, ParticleGenerator::Constant, 0);
    gen->setLifetime(ParticleGenerator::TimeExistedLifetime, 0);
    return gen;
}

ParticleGenerator::Ptr ParticleGeneratorFactory::create(Preset type, ParticleGenerator::LifetimeType lifetimeType, float lifetime) {
    switch (type) {
        case Smoke:
            return createSmoke(lifetime);

        default:
            cout << "Warning: Invalid ParticleGeneratorFactory::Preset " << type << endl;
            return createBlank();
    }
}
