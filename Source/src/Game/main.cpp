#include "Game/Core/Game.hpp"
#include "Shared/DebugOverlays.hpp"
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));
    DebugOverlays::toggleOverlay(DebugOverlays::None);
    Game::get().run();
	return 0;
}
