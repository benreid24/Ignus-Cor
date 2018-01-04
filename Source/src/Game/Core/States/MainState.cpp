#include "Game/Core/States/MainState.hpp"
#include "Game/Core/Game.hpp"
using namespace std;

bool MainState::doState() {
    while (true) {
        if (handleWindow())
            return true;

		game->mapManager.update();
		game->entityManager.update();

        game->window.clear();
        game->mapManager.render();
        game->window.display();

        ensureFps();
    }
}

MainState::MainState(Game* g) : BaseState(g, nullptr) {
    g->mapManager.loadMap("test");
    game->entityManager.add(game->player);
}

MainState::Ptr MainState::create(Game* g) {
    return Ptr(new MainState(g));
}
