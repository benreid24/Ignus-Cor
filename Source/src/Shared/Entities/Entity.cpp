#include "Shared/Entities/Entity.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Entities/EntityBehavior.hpp"
#include "Shared/Util/UUID.hpp"
#include <sstream>
using namespace std;
using namespace sf;

Clock Entity::timer;
EntityManager* Entity::entityManager = nullptr;
int UUID::nextUuid = 1;

Entity::Entity(string nm, EntityPosition pos, string gfx1, string gfx2) {
	graphics.load(gfx1,gfx2);
	position = pos;
	name = nm;
	position.dir = 0;
	uuid = UUID::create();
	lTime = Entity::timer.getElapsedTime().asSeconds();
	speed[0] = 64;
	speed[1] = 128;
	boundingBox = FloatRect(Vector2f(0, 12), graphics.getSize());
	behavior = nullptr;
}

Entity::~Entity() {
    if (behavior != nullptr)
        delete behavior;
}

EntityPosition Entity::getPosition() {
	return position;
}

FloatRect Entity::getBoundingBox() {
	return FloatRect(
                  position.coords.x+boundingBox.left,
                  position.coords.y+boundingBox.top,
                  boundingBox.width,
                  boundingBox.height);
}

void Entity::setPositionAndDirection(EntityPosition pos) {
	position = pos;
}

void Entity::shift(sf::Vector2f amount, bool truncate) {
	position.coords += amount;
	if (truncate) {
        position.coords.x = int(position.coords.x/32)*32;
        position.coords.y = int(position.coords.y/32)*32;
	}
}

string Entity::getName() {
	return name;
}

void Entity::notifyAttacked(Ptr attacker) {
    cout << getIdString() << " ATTACKED BY " << attacker->getIdString() << endl;
    if (behavior != nullptr)
        behavior->notifyAttacked(attacker);
}

void Entity::notifyInteracted(Ptr user) {
    cout << getIdString() << " INTERACTED BY " << user->getIdString() << endl;
    if (behavior != nullptr)
        behavior->notifyInteracted(user);
}

void Entity::notifyCombatNearby(List combatants) {
    if (behavior != nullptr)
        behavior->notifyCombatNearby(combatants);
}

string Entity::getIdString() {
    stringstream ss;
    ss << "<" << getType() << ":" << getName() << "(uuid: " << uuid << ")>";
    return ss.str();
}

void Entity::render(sf::RenderTarget& target, sf::Vector2f camPos) {
	graphics.render(target,position.coords-camPos);
	bubble.render(target,position.coords-camPos);
}

void Entity::move(int d, bool fast, float elapsedTime) {
	EntityPosition newPos = position;
	position.dir = d;

	elapsedTime = (elapsedTime==0)?(Entity::timer.getElapsedTime().asSeconds()-lTime):(elapsedTime);
	int i = int(fast);
	float dist = speed[i]*elapsedTime;

	switch (position.dir) {
	case 0:
        newPos.coords.y -= dist;
        break;
	case 1:
		newPos.coords.x += dist;
		break;
	case 2:
		newPos.coords.y += dist;
		break;
	case 3:
		newPos.coords.x -= dist;
		break;
	default:
		cout << "Warning: Entity::move received an invalid direction!\n";
	}

	if (newPos.coords != position.coords) {
		EntityPosition oldPosOffset = position;
		EntityPosition newPosOffset = newPos;
		oldPosOffset.coords += Vector2f(boundingBox.left, boundingBox.top);
		Vector2f size = Vector2f(boundingBox.width, boundingBox.height);
		if (Entity::entityManager->canMove(this, oldPosOffset, newPosOffset, size)) {
			graphics.setMoving(position.dir, fast);
			swap(position, newPos); //newPos is now old pos
			Entity::entityManager->updatePosition(this, newPos);
		}
	}
}

void Entity::update() {
	lTime = Entity::timer.getElapsedTime().asSeconds();
}

void Entity::setEntityManager(EntityManager* em) {
	entityManager = em;
}
