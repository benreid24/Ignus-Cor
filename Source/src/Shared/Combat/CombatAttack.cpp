#include "Shared/Combat/CombatAttack.hpp"
using namespace std;

CombatAttack::CombatAttack(const string& nm, const string& desc, double pwr, const list<CombatEffect>& efx, const string& anim) {
    name = nm;
    description = desc;
    power = pwr;
    effects = efx;
    animation = anim;
}

string CombatAttack::getName() {
    return name;
}

string CombatAttack::getDescription() {
    return description;
}

double CombatAttack::getPower() {
    return power;
}

list<CombatEffect> CombatAttack::getEffects() {
    return effects;
}

string CombatAttack::getAnimation() {
    return animation;
}
