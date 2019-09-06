#include "Shared/Combat/CombatArmor.hpp"
using namespace std;

CombatArmor::CombatArmor() {
    name = "Clothing";
    description = "Just barely a step above being naked. These thin garbs offer no protection whatsoever";
    damageResist = 0;
}

CombatArmor::CombatArmor(const string& nm, const string& desc, double dr, const list<CombatArmorEffect>& efx) {
    name = nm;
    description = desc;
    damageResist = dr;
    effects = efx;
}

string CombatArmor::getName() const {
    return name;
}

string CombatArmor::getDescription() const {
    return description;
}

double CombatArmor::getDamageResist() const {
    return damageResist;
}

list<CombatArmorEffect> CombatArmor::getEffects() const {
    return effects;
}
