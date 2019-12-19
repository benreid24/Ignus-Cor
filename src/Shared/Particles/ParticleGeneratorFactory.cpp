#include "Shared/Particles/ParticleGeneratorFactory.hpp"
using namespace std;

namespace {
const vector<string> presets = {
    "None",
    "Smoke"
};
}

ParticleGenerator::Ptr ParticleGeneratorFactory::createSmoke(ParticleGenerator::LifetimeType lifetimeType, float lifetime) {
    ParticleGenerator::ValueWindow spRad(0,7), spAng(0,360), spVel(2,5), spDir(-85,-110);
    ParticleGenerator::Ptr gen = ParticleGenerator::create(spRad, spAng, spVel, spDir);
    gen->setGenerationRate(15, ParticleGenerator::Linear, 0);
    gen->setLifetime(lifetimeType, lifetime);

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
            return createSmoke(lifetimeType, lifetime);

        case None:
            return createBlank();

        default:
            cout << "Warning: Invalid ParticleGeneratorFactory::Preset " << type << endl;
            return createBlank();
    }
}

const vector<string>& ParticleGeneratorFactory::getAllPresets() {
    return presets;
}

ParticleGeneratorFactory::Preset ParticleGeneratorFactory::getPresetFromName(const string& name) {
    for (unsigned int i = 0; i<presets.size(); ++i) {
        if (presets[i] == name)
            return Preset(i);
    }
    return None;
}
