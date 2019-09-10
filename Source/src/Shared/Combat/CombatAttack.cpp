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

CombatAttack::CombatAttack(const string& nm, const string& desc, double pwr, float del, const CombatEffect::List& efx,
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
                           const CombatEffect::List& efx, const string& anim, ParticleGeneratorFactory::Preset parts, float ptime,
                           double rng, double spd, const string& impactAnim, ParticleGeneratorFactory::Preset impactParts, float iptime)
: CombatAttack(nm, desc, pwr, del, efx, anim, parts, ptime) {
    type = Ranged;
    range = rng;
    speed = spd;
    impactAnimation = impactAnim;
    impactParticleGenerator = impactParts;
    impactParticlePersistTime = iptime;
}

CombatAttack::CombatAttack(File& file) {
    type = Type(file.get<uint16_t>());
    name = file.getString();
    description = file.getString();
    power = file.get<uint32_t>();
    delaySeconds = double(file.get<uint16_t>()) / 100.0;
    animation = file.getString();
    particleGenerator = ParticleGeneratorFactory::Preset(file.get<uint8_t>());
    particlePersistTime = double(file.get<uint16_t>()) / 100.0;

    int numEffects = file.get<uint16_t>();
    for (int i = 0; i<numEffects; ++i) {
        int tp = file.get<uint8_t>();
        double intense = double(file.get<uint16_t>()) / 100.0;
        double chance = double(file.get<uint16_t>())/100.0;
        double duration = double(file.get<uint32_t>())/1000.0;
        effects.push_back(CombatEffect(tp, intense, chance, duration));
    }

    if (type == Ranged) {
        impactAnimation = file.getString();
        speed = file.get<uint16_t>();
        range = file.get<uint16_t>();
        impactParticleGenerator = ParticleGeneratorFactory::Preset(file.get<uint8_t>());
        impactParticlePersistTime = double(file.get<uint16_t>()) / 100.0;
    }
}

void CombatAttack::save(File& file) const {
    file.write<uint16_t>(type);
    file.writeString(name);
    file.writeString(description);
    file.write<uint32_t>(power);
    file.write<uint16_t>(delaySeconds * 100);
    file.writeString(animation);
    file.write<uint8_t>(particleGenerator);
    file.write<uint16_t>(particlePersistTime * 100);

    file.write<uint16_t>(effects.size());
    for (auto i = effects.begin(); i!=effects.end(); ++i) {
        file.write<uint8_t>(i->getType());
        file.write<uint16_t>(i->getIntensity() * 100);
        file.write<uint16_t>(i->getChance() * 100);
        file.write<uint32_t>(i->getDuration() * 1000);
    }

    if (type == Ranged) {
        file.writeString(impactAnimation);
        file.write<uint16_t>(speed);
        file.write<uint16_t>(range);
        file.write<uint8_t>(impactParticleGenerator);
        file.write<uint16_t>(impactParticlePersistTime * 100);
    }
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

const CombatEffect::List& CombatAttack::getEffects() const {
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

CombatAttack::ConstPtr CombatAttack::toExplosionAttack() const {
    return make_shared<CombatAttack>(
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
