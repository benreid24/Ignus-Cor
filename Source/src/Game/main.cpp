#include "Game/Core/Game.hpp"
#include "Shared/DebugOverlays.hpp"
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));
    DebugOverlays::toggleOverlay(DebugOverlays::None);
    Properties::PrimaryMenuFont.loadFromFile(Properties::FontPath+"Abea.ttf");
    Properties::ConversationFont.loadFromFile(Properties::FontPath+"Abea.ttf");
    Game::get().run();
	return 0;
}
