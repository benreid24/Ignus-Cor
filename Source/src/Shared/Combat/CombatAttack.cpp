#include "Shared/Combat/CombatAttack.hpp"
#include "Shared/Entities/Entity.hpp"
using namespace std;

CombatAttack::CombatAttack() {
    type = Melee;
    name = "Punch";
    description = "ONE PUNCH!!";
    power = 15;
    animation = "Attacks/punch.anim";
}

CombatAttack::CombatAttack(const string& nm, const string& desc, double pwr, const list<CombatEffect>& efx, const string& anim) {
    type = Melee;
    name = nm;
    description = desc;
    power = pwr;
    effects = efx;
    animation = anim;
}

CombatAttack::CombatAttack(const string& nm, const string& desc, double pwr, const list<CombatEffect>& efx, const string& anim,
                                       double rng, double spd, const string& impactAnim)
: CombatAttack(nm, desc, pwr, efx, anim) {
    type = Ranged;
    range = rng;
    speed = spd;
    impactAnimation = impactAnim;
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

list<CombatEffect> CombatAttack::getEffects() const {
    return effects;
}

string CombatAttack::getAnimation() const {
    return animation;
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

CombatAttack CombatAttack::toExplosionAttack() const {
    return CombatAttack(
        name+"-explosion",
        description,
        power, //TODO - separate power/effects for explosion?
        effects,
        impactAnimation
    );
}
