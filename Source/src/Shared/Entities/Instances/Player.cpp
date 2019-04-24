#include "Shared/Entities/Instances/Player.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Maps/MapManager.hpp"
#include "Shared/Properties.hpp"
using namespace std;
using namespace sf;

Player::Player() :
	Entity("player name",
		   EntityPosition(Vector2f(5,5), "test.map", EntityPosition::Direction::Up),
		   Properties::AnimationPath+"Entities/BoyPlayer/Walk",
		   Properties::AnimationPath+"Entities/BoyPlayer/Run") {
	speed[0] = 64;
	speed[1] = 128;
}

Player::Ptr Player::create() {
	static Ptr player(new Player());
	return player;
}

void Player::update() {
	bool running = Keyboard::isKeyPressed(Keyboard::Space);
    bool moved = true;

	if (Keyboard::isKeyPressed(Keyboard::W))
		move(EntityPosition::Up, running);
	else if (Keyboard::isKeyPressed(Keyboard::A))
		move(EntityPosition::Left, running);
	else if (Keyboard::isKeyPressed(Keyboard::S))
		move(EntityPosition::Down, running);
	else if (Keyboard::isKeyPressed(Keyboard::D))
		move(EntityPosition::Right, running);
	else
		moved = false;

	if (Keyboard::isKeyPressed(Keyboard::E))
        interact();

	MapManager::get()->updateRenderPosition(position.coords);

	Entity::update();
}

const string Player::getType() {
	return "Player";
}
