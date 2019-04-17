#include "Game/Core/States/MainState.hpp"
#include "Game/Core/Game.hpp"
using namespace std;

bool MainState::doState() {
    while (true) {
        if (handleWindow())
            return true;

		Game::get()->mapManager.update();
		Game::get()->entityManager.update();

        Game::get()->window.clear();
        Game::get()->mapManager.render();
        Game::get()->window.display();

        ensureFps();
    }
}

MainState::MainState() : BaseState(nullptr) {
    Game::get()->mapManager.loadMap("test");
    Game::get()->entityManager.add(Game::get()->player);
}

MainState::Ptr MainState::create() {
    return Ptr(new MainState());
}
