#include "Shared/Combat/CombatAttack.hpp"
#include "Shared/Entities/Entity.hpp"
using namespace std;

CombatAttack::CombatAttack() {
    type = Melee;
    name = "Punch";
    description = "ONE PUNCH!!";
    power = 15;
    delaySeconds = 0.5;
    animation = "Attacks/punch.anim";
}

CombatAttack::CombatAttack(const string& nm, const string& desc, double pwr, float del, const list<CombatEffect>& efx,
                           const string& anim, ParticleGeneratorFactory::Preset parts, float ptime) {
    type = Melee;
    name = nm;
    description = desc;
    power = pwr;
    delaySeconds = del;
    effects = efx;
    animation = anim;
    particleGenerator = parts;
    particlePersistTime = ptime;
}

CombatAttack::CombatAttack(const string& nm, const string& desc, double pwr, float del,
                           const list<CombatEffect>& efx, const string& anim, ParticleGeneratorFactory::Preset parts, float ptime,
                           double rng, double spd, const string& impactAnim, ParticleGeneratorFactory::Preset impactParts, float iptime)
: CombatAttack(nm, desc, pwr, del, efx, anim, parts, ptime) {
    type = Ranged;
    range = rng;
    speed = spd;
    impactAnimation = impactAnim;
    impactParticleGenerator = impactParts;
    impactParticlePersistTime = iptime;
}

CombatAttack::Type CombatAttack::getType() const {
    return type;
}

string CombatAttack::getName() const {
    return name;
}

string CombatAttack::getDescription() const {
    return description;
}

double CombatAttack::getPower() const {
    return power;
}

float CombatAttack::getAttackDelay() const {
    return delaySeconds;
}

list<CombatEffect> CombatAttack::getEffects() const {
    return effects;
}

string CombatAttack::getAnimation() const {
    return animation;
}

ParticleGeneratorFactory::Preset CombatAttack::getParticleType() const {
    return particleGenerator;
}

float CombatAttack::getParticlePersistanceTime() const {
    return particlePersistTime;
}

double CombatAttack::getRange() const {
    return range;
}

double CombatAttack::getSpeed() const {
    return speed;
}

string CombatAttack::getImpactAnimation() const {
    return impactAnimation;
}

float CombatAttack::getImpactParticlePersistance() const {
    return impactParticlePersistTime;
}

ParticleGeneratorFactory::Preset CombatAttack::getImpactParticles() const {
    return impactParticleGenerator;
}

CombatAttack CombatAttack::toExplosionAttack() const {
    return CombatAttack(
        name+"-explosion",
        description,
        power, //TODO - separate power/effects for explosion?
        delaySeconds,
        effects,
        impactAnimation,
        impactParticleGenerator,
        impactParticlePersistTime
    );
}
