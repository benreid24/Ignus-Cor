#include "Shared/Objects/ItemEffect.hpp"

namespace {

std::string invalid = "INVALID";
std::vector<std::string> effectStrings = {
	"None",
	"Damage Health",
	"Restore Health",
	"Restore Magic",
	"Damage Magic"
};

}

ItemEffect::ItemEffect(Type tp, int intense) : type(tp), intensity(intense) {}

ItemEffect::ItemEffect(int effect) : ItemEffect(Type(effect)) {}

ItemEffect::ItemEffect(const std::string& desc, int intense) {
    intensity = intense;
    for (unsigned int i = 0; i<effectStrings.size(); ++i) {
        if (effectStrings[i] == desc) {
            type = Type(i);
            return;
        }
    }
    type = None;
}

const std::string& ItemEffect::getDescription() const {
	unsigned int t = type;
	if (t<effectStrings.size())
        return effectStrings[t];
    return invalid;
}

const std::vector<std::string>& ItemEffect::getAllEffects() {
    return effectStrings;
}
