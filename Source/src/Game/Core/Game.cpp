#include "Game/Core/Game.hpp"
#include "Game/Core/States/MainState.hpp"
#include "Shared/Properties.hpp"
using namespace std;
using namespace sf;

Game::Game() {
    inFocus = true;
	window.create(VideoMode(Properties::ScreenWidth, Properties::ScreenHeight, 32), "Ignis Cor", Style::Close|Style::Resize|Style::Titlebar);
}

Game::~Game() {
    window.close();
}

void Game::run() {
	mapManager.setPointers(this, player, &entityManager, &soundEngine, &playlist);
    MainState::Ptr state = MainState::create(this);
    state->start();
}
