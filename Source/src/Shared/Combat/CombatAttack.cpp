#include "Shared/Combat/CombatAttack.hpp"
#include "Shared/Entities/Entity.hpp"
using namespace std;

CombatAttack::Ref CombatAttack::fromItem(Item::ConstPtr item) {
    return std::dynamic_pointer_cast<const CombatAttack, const Item>(item);
}

CombatAttack::CombatAttack(int id, const string& nm, const string& desc,
         const ItemEffect::List& fx, int val, const string& mpImg,
         const string& mnImg, double pwr, float dly,
         ParticleGeneratorFactory::Preset parts, float ptime)
: Item (id, Item::Weapon, nm, desc, fx, val, mpImg, mnImg) {
    type = Melee;
    power = pwr;
    delaySeconds = dly;
    particleGenerator = parts;
    particlePersistTime = ptime;
}

CombatAttack::CombatAttack(int id, const string& nm, const string& desc,
         const ItemEffect::List& fx, int val, const string& mpImg,
         const string& mnImg, double pwr, float dly,
         ParticleGeneratorFactory::Preset parts, float ptime,
         Item::Category cat, double rng, double spd,
         const string& impactAnim,
         ParticleGeneratorFactory::Preset impactParts, float iptime)
: Item(id, cat, nm, desc, fx, val, mpImg, mnImg) {
    type = Ranged;
    power = pwr;
    delaySeconds = dly;
    particleGenerator = parts;
    particlePersistTime = ptime;
    range = rng;
    speed = spd;
    impactAnimation = impactAnim;
    impactParticleGenerator = impactParts;
    impactParticlePersistTime = iptime;
}

void CombatAttack::p_save(File& file) const {
    file.write<uint8_t>(type);
    file.write<uint32_t>(power);
    file.write<uint16_t>(delaySeconds * 100);
    file.write<uint8_t>(particleGenerator);
    file.write<uint16_t>(particlePersistTime * 100);

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

double CombatAttack::getPower() const {
    return power;
}

float CombatAttack::getAttackDelay() const {
    return delaySeconds;
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

CombatAttack::Ref CombatAttack::toExplosionAttack() const {
    return Ref(
        new CombatAttack(
            id,
            name+"-explosion",
            description,
            effects,
            0, impactAnimation, "",
            power, //TODO - separate power/effects for explosion?
            delaySeconds,
            impactParticleGenerator,
            impactParticlePersistTime
        )
    );
}
