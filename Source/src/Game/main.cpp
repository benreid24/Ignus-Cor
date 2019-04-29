#include "Game/Core/Game.hpp"
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));
    Game::get()->run();
	return 0;
}
