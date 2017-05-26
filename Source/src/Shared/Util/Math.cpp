#include "Shared/Util/Math.hpp"
#include <cstdlib>
#include <algorithm>

int Random(int mn, int mx) {
	if (mn<mx)
		std::swap(mn,mx);
	return (rand()%(mx-mn))+mn;
}
