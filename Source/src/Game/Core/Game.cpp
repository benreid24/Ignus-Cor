#include "Game/Core/Game.hpp"
#include "Game/Core/States/MainState.hpp"
#include "Game/Core/PlayerInput.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Entities/Instances/Player.hpp"
using namespace std;
using namespace sf;

Game::Game() {
    inFocus = true;
	window.create(VideoMode(Properties::ScreenWidth, Properties::ScreenHeight, 32), "Ignis Cor", Style::Close|Style::Resize|Style::Titlebar);

	player = Player::create();
	MapManager::get()->setPlayer(player);

	eventDispatcher.addGlobalObserver(&PlayerInput::get());
}

Game::~Game() {
    window.close();
}

void Game::run() {
    MainState::Ptr state = MainState::create();
    state->start();
}

void Game::runNewState(BaseState::Ptr state) {
    if (states.size() > 0)
        states.top()->runState(state);
}

Game& Game::get() {
    static Game game;
    return game;
}
