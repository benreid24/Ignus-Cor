#include "Game/Core/Game.hpp"
#include "Game/Core/States/MainState.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Entities/Instances/Player.hpp"
using namespace std;
using namespace sf;

Game::Game() : entityManager(this) {
    inFocus = true;
	window.create(VideoMode(Properties::ScreenWidth, Properties::ScreenHeight, 32), "Ignis Cor", Style::Close|Style::Resize|Style::Titlebar);

	mapManager.setPointers(this, player, &entityManager, &soundEngine, &playlist);
	player = Player::create();
	Entity::setEntityManager(&entityManager);
}

Game::~Game() {
    window.close();
}

void Game::run() {
    MainState::Ptr state = MainState::create(this);
    state->start();
}
