#include "Game/Core/States/MainState.hpp"
#include "Game/Core/Game.hpp"
using namespace std;

bool MainState::doState() {
    while (true) {
        if (handleWindow())
            return true;
        if (runImmediate())
            return true;

		MapManager::get()->update();
		EntityManager::get()->update();

        Game::get().window.clear();
        MapManager::get()->render();
        Game::get().window.display();

        ensureFps();
    }
}

MainState::MainState() : BaseState(nullptr) {
    MapManager::get()->loadMap("test.map");
    EntityManager::get()->add(Game::get().player);
}

MainState::Ptr MainState::create() {
    return Ptr(new MainState());
}
