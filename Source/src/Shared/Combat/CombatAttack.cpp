#include "Shared/Combat/CombatAttack.hpp"
#include "Shared/Entities/Entity.hpp"
using namespace std;

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

void CombatAttack::apply(Entity* attacker, Entity* defender) const {
    EntityStats& atkStats = attacker->getStats();
    EntityStats& defStats = defender->getStats();
    const CombatArmor& armor = defender->getArmor();

    double ld = atkStats.level - defStats.level;
    double lvlMult = (ld>0) ? (ld*ld) : (2.5/abs(ld));
    double atkPower = power * lvlMult;
    double damage = atkPower - armor.getDamageResist();
    defStats.health -= damage;
    //TODO - effects and create particle generators from them
}
