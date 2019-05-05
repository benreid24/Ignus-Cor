#include "Shared/Util/Util.hpp"
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <random>
using namespace std;

namespace {
std::random_device rngdev;
std::mt19937 rng(rngdev());
}

int randomInt(int mn, int mx) {
	if (mn>mx)
		std::swap(mn,mx);
    std::uniform_int_distribution<std::mt19937::result_type> dist(mn,mx);
	return dist(rng);
}

string intToString(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}

int stringToInt(string s)
{
    stringstream ss;
    ss << s;
    int i = 0;
    ss >> i;
    return i;
}
