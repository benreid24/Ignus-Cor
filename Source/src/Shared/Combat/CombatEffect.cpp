#include "Shared/Combat/CombatEffect.hpp"
#include <vector>
using namespace std;

namespace {
const string invalid = "INVALID";
const vector<pair<string, string> > effects = {
	make_pair("None", ""),
	make_pair("Fire", "Damages health over time with fire damage"),
	make_pair("Freeze", "Damages health and slows movement speed"),
	make_pair("Electrocute", "Damages health and magicka"),
	make_pair("Poison", "Damages health over time")
};
}

CombatEffect::CombatEffect(Type tp, double ints, double odds, double dur)
: type(tp), intensity(ints), chance(odds), duration(dur) {}

CombatEffect::CombatEffect(int tp, double ints, double odds, double dur)
: CombatEffect(Type(tp), ints, odds, dur) {}

const string& CombatEffect::getName() const {
    unsigned int i = type;
    if (i < effects.size())
        return effects[i].first;
    return invalid;
}

const string& CombatEffect::getDescription() const {
    unsigned int i = type;
    if (i < effects.size())
        return effects[i].second;
    return invalid;
}

CombatEffect::Type CombatEffect::getType() const {
    return type;
}

double CombatEffect::getIntensity() const {
    return intensity;
}

double CombatEffect::getChance() const {
    return chance;
}

double CombatEffect::getDuration() const {
    return duration;
}
