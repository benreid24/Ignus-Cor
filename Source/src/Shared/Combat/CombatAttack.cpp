#include "Shared/Combat/CombatAttack.hpp"
#include "Shared/Entities/Entity.hpp"
using namespace std;

CombatAttack::CombatAttack() {
    name = "Punch";
    description = "ONE PUNCH!!";
    power = 15;
    animation = "Attacks/punch.anim";
}

CombatAttack::CombatAttack(const string& nm, const string& desc, double pwr, const list<CombatEffect>& efx, const string& anim) {
    name = nm;
    description = desc;
    power = pwr;
    effects = efx;
    animation = anim;
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