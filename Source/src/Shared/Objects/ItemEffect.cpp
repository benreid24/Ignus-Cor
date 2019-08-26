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

const std::string& ItemEffect::getDescription() const {
	unsigned int t = type;
	if (t<effectStrings.size())
        return effectStrings[t];
    return invalid;
}

const std::vector<std::string>& ItemEffect::getAllBaseEffects() {
    return effectStrings;
}
