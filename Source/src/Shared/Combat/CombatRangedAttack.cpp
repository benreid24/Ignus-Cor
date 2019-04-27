#include "Shared/Combat/CombatRangedAttack.hpp"
using namespace std;

CombatRangedAttack::CombatRangedAttack(const string& nm, const string& desc, double pwr, const list<CombatEffect>& efx, const string& anim,
                                       double rng, double spd, const string& impactAnim, double explosionRad)
: CombatAttack(nm, desc, pwr, efx, anim) {
    range = rng;
    speed = spd;
    impactAnimation = impactAnim;
    explosionRadius = explosionRad;
}

double CombatRangedAttack::getRange() {
    return range;
}

double CombatRangedAttack::getSpeed() {
    return speed;
}

string CombatRangedAttack::getImpactAnimation() {
    return impactAnimation;
}

double CombatRangedAttack::getExplosionRadius() {
    return explosionRadius;
}
