#include "Shared/Entities/Instances/Player.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Maps/MapManager.hpp"
#include "Shared/Properties.hpp"
#include "Game/Core/Game.hpp"
#include "Shared/Data/AttackDB.hpp"
#include <cmath>
using namespace std;
using namespace sf;

Player::Player() :
	CombatEntity("Test Player",
		   EntityPosition(Vector2f(205,705), "test.map", EntityPosition::Direction::Up),
		   "Entities/BoyPlayer/Walk",
		   "Entities/BoyPlayer/Run") {
	speed[0] = 64;
	speed[1] = 128;
	boundingBox = FloatRect(0, 12, graphics.getSize().x, graphics.getSize().y-12);
	weapon = AttackDB::get().getAttack(-1);
}

Player::Ptr Player::create() {
	static Ptr player(new Player());
	return player;
}

void Player::beforeTimerUpdate() {
	bool running = Keyboard::isKeyPressed(Keyboard::LShift);
	if (Keyboard::isKeyPressed(Keyboard::W))
		move(EntityPosition::Up, running);
	if (Keyboard::isKeyPressed(Keyboard::A))
		move(EntityPosition::Left, running);
	if (Keyboard::isKeyPressed(Keyboard::S))
		move(EntityPosition::Down, running);
	if (Keyboard::isKeyPressed(Keyboard::D))
		move(EntityPosition::Right, running);

    if (Keyboard::isKeyPressed(Keyboard::Num1))
        weapon = AttackDB::get().getAttack(-1);
	if (Keyboard::isKeyPressed(Keyboard::Num2))
        weapon = AttackDB::get().getAttack(-2);

    if (Mouse::isButtonPressed(Mouse::Left))
        doAttack(getAttackDirection());

	if (Keyboard::isKeyPressed(Keyboard::E))
        interact();

	MapManager::get()->updateRenderPosition(position.coords);
}

const string Player::getType() const {
	return "Player";
}

int Player::getAttackDirection() const {
    #ifdef GAME
    Vector2f scrnCenter(Properties::ScreenWidth/2, Properties::ScreenHeight/2);
    Vector2f mousePos = Vector2f(Mouse::getPosition(Game::get().window));
    Vector2f diff = mousePos - scrnCenter;
    return atan2(diff.y, diff.x) * 180 / 3.1415926;
    #else
    return 0;
    #endif
}
