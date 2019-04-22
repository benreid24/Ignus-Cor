#include "Shared/Entities/Instances/Player.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Properties.hpp"
using namespace std;
using namespace sf;

Player::Player() :
	Entity("player name",
		   EntityPosition(Vector2f(5,5), "test.map", 0),
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
		move(0, running);
	else if (Keyboard::isKeyPressed(Keyboard::A))
		move(3, running);
	else if (Keyboard::isKeyPressed(Keyboard::S))
		move(2, running);
	else if (Keyboard::isKeyPressed(Keyboard::D))
		move(1, running);
	else
		moved = false;
	Entity::entityManager->updateRenderPosition(position.coords);

	Entity::update();
}

const string Player::getType() {
	return "Player";
}
