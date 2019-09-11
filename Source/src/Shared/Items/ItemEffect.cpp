#include "Shared/Items/ItemEffect.hpp"
using namespace std;

namespace {
const string invalid = "INVALID";
const vector<pair<string, string> > effects = {
	make_pair("None", ""),
	make_pair("Damage Health", "Damages the health of the user"),
	make_pair("Restore Health", "Replenishes HP"),
	make_pair("Restore Magic", "Restores magicka"),
	make_pair("Damage Magic", "Depletes the magicka of the user")
};
}

ItemEffect::ItemEffect(Type tp, int intense, float odds)
: type(tp), intensity(intense), chance(odds) {}

ItemEffect::ItemEffect(int effect, int intense, float odds)
: ItemEffect(Type(effect), intense, odds) {}

ItemEffect::ItemEffect(const string& name, int intense, float odds) {
    intensity = intense;
    chance = odds;
    for (unsigned int i = 0; i<effects.size(); ++i) {
        if (effects[i].first == name) {
            type = Type(i);
            return;
        }
    }
    type = None;
}

const string& ItemEffect::getDescription() const {
	unsigned int t = type;
	if (t<effects.size())
        return effects[t].second;
    return invalid;
}

const string& ItemEffect::getName() const {
	unsigned int t = type;
	if (t<effects.size())
        return effects[t].first;
    return invalid;
}

float ItemEffect::getOdds() const {
    return chance;
}

const vector<pair<string, string> >& ItemEffect::getAllEffects() {
    return effects;
}
