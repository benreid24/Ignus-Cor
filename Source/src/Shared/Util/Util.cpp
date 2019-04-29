#include "Shared/Util/Util.hpp"
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <iostream>
using namespace std;

int randomInt(int mn, int mx) {
	if (mn>mx)
		std::swap(mn,mx);
	return (rand()%(mx-mn))+mn;
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
