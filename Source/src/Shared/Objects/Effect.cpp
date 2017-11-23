#include "Shared/Objects/Effect.hpp"

namespace Effects{

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

bool isEffect(Effect value, Effect test) {
	return (uint32_t(value)&uint32_t(test)) != 0;
}

std::string getEffectString(Effect eff, std::string sep) {
	std::string ret;
	bool prependSep = false;
	uint32_t e = uint32_t(eff);

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

}
