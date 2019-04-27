#include "Shared/Combat/CombatRangedAttack.hpp"
using namespace std;

CombatRangedAttack::CombatRangedAttack(const string& nm, const string& desc, double pwr, const list<CombatEffect>& efx, const string& anim,
                                       double rng, double spd, const string& impactAnim)
: CombatAttack(nm, desc, pwr, efx, anim) {
    range = rng;
    speed = spd;
    impactAnimation = impactAnim;
}

double CombatRangedAttack::getRange() const {
    return range;
}

double CombatRangedAttack::getSpeed() const {
    return speed;
}

string CombatRangedAttack::getImpactAnimation() const {
    return impactAnimation;
}
