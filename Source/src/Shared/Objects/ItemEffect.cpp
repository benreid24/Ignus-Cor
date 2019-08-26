#include "Shared/Objects/ItemEffect.hpp"

namespace {

std::vector<std::string> effectStrings = {
	"None",
	"Damage Health",
	"Restore Health",
	"Restore Magic",
	"Damage Magic"
};

int effectMap[33] = {
	0,
	1 << 1,
	1 << 2,
	1 << 3,
	1 << 4,
	1 << 5,
	1 << 6,
	1 << 7,
	1 << 8,
	1 << 9,
	1 << 10,
	1 << 11,
	1 << 12,
	1 << 13,
	1 << 14,
	1 << 15,
	1 << 16,
	1 << 17,
	1 << 18,
	1 << 19,
	1 << 20,
	1 << 21,
	1 << 22,
	1 << 23,
	1 << 24,
	1 << 25,
	1 << 26,
	1 << 27,
	1 << 28,
	1 << 29,
	1 << 30,
	1 << 31,
	1 << 31,
};

}

ItemEffect::ItemEffect(Type tp) : type(tp) {}

ItemEffect::ItemEffect(int effect) : ItemEffect(Type(effect)) {}

bool ItemEffect::isEffect(Type test) const {
	return (uint32_t(type)&uint32_t(test)) != 0;
}

ItemEffect::Type ItemEffect::getEffect() const { return type; }

std::string ItemEffect::getDescription(const std::string& sep) const {
	std::string ret;
	bool prependSep = false;
	uint32_t e = uint32_t(type);

	for (unsigned int i = 0; i<effectStrings.size(); ++i) {
		uint32_t v = e >> i;
		v &= 1;
		if (v!=0) {
			if (prependSep)
				ret += sep;
			ret += effectStrings[i];
			prependSep = true;
		}
	}
	return ret;
}

ItemEffect::Type ItemEffect::getTypeFromIndex(unsigned int i) {
    if (i <= 32)
        return ItemEffect::Type(effectMap[i]);
    return ItemEffect::None;
}

unsigned int ItemEffect::getIndexFromType(Type type) {
    for (int i = 0; i<=32; ++i) {
        if (effectMap[i] == type)
            return i;
    }
    return 0;
}

const std::vector<std::string>& ItemEffect::getAllBaseEffects() {
    return effectStrings;
}
