#include "Shared/Objects/Effect.hpp"

namespace Effects{

bool isEffect(Effect value, Effect test) {
	return (uint32_t(value)&uint32_t(test)) != 0;
}

}
