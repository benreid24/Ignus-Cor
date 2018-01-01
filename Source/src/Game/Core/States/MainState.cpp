#include "Game/Core/States/MainState.hpp"
#include "Game/Core/Game.hpp"
using namespace std;

bool MainState::doState() {
    while (true) {
        if (handleWindow())
            return true;

        game->window.clear();
        game->window.display();

        ensureFps();
    }
}

MainState::MainState(Game* g) : BaseState(g, nullptr) {
    //Init game?
}

MainState::Ptr MainState::create(Game* g) {
    return Ptr(new MainState(g));
}
