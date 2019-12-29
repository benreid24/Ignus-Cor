#include "Shared/Entities/Instances/Player.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Maps/MapManager.hpp"
#include "Shared/Properties.hpp"
#include "Game/Core/Game.hpp"
#include "Game/Core/PlayerInput.hpp"
#include "Game/Core/States/PauseState.hpp"
#include <cmath>
using namespace std;
using namespace sf;

Player::Player() :
	CombatEntity("Test Player",
		   EntityPosition(Vector2f(805,805), "test.map", EntityPosition::Direction::Up),
		   "Entities/BoyPlayer/Walk",
		   "Entities/BoyPlayer/Run") {
	speed[0] = 64;
	speed[1] = 128;
	boundingBox = FloatRect(0, 12, graphics.getSize().x, graphics.getSize().y-12);
	weapon = CombatAttack::fromItem(ItemDB::get().getItem(Item::DefaultWeapon));
}

Player::Ptr Player::create() {
	static Ptr player(new Player());
	return player;
}

void Player::beforeTimerUpdate() {
    #ifdef GAME
	bool running = PlayerInput::get().inputActive(PlayerInput::Sprint);
	if (PlayerInput::get().inputActive(PlayerInput::Up))
		move(EntityPosition::Up, running);
	if (PlayerInput::get().inputActive(PlayerInput::Left))
		move(EntityPosition::Left, running);
	if (PlayerInput::get().inputActive(PlayerInput::Down))
		move(EntityPosition::Down, running);
	if (PlayerInput::get().inputActive(PlayerInput::Right))
		move(EntityPosition::Right, running);

    if (Keyboard::isKeyPressed(Keyboard::Num1)) //TODO - switch weapon controls in player input
        weapon = CombatAttack::fromItem(ItemDB::get().getItem(Item::DefaultWeapon));
	if (Keyboard::isKeyPressed(Keyboard::Num2))
        weapon = CombatAttack::fromItem(ItemDB::get().getItem(Item::DefaultSpell));

    if (PlayerInput::get().inputActive(PlayerInput::Attack))
        doAttack(getAttackDirection());

	if (PlayerInput::get().inputActive(PlayerInput::Interact))
        interact();

    if (PlayerInput::get().inputActive(PlayerInput::Pause))
        Game::get().runNewState(PauseState::create());
    #endif

	MapManager::get().updateRenderPosition(position.coords);
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
