#include <cstdlib>
#include <ctime>

#include "Game/Core/Game.hpp"
#include "Shared/DebugOverlays.hpp"
#include "Shared/Properties.hpp"

using namespace std;

int main() {
    srand(time(0));
    
    Properties::PrimaryMenuFont.loadFromFile(Properties::FontPath+"Abea.ttf");
    Properties::SecondaryMenuFont.loadFromFile(Properties::FontPath+"Abea.ttf");
    Properties::ConversationFont.loadFromFile(Properties::FontPath+"Abea.ttf");

    DebugOverlays::toggleOverlay(DebugOverlays::None);
    Game::get().run();

	return 0;
}
