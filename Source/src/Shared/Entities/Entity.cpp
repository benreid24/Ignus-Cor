#include "Shared/Entities/Entity.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Entities/EntityBehavior.hpp"
#include "Shared/Maps/MapManager.hpp"
#include "Shared/Util/UUID.hpp"
#include <sstream>
using namespace std;
using namespace sf;

Clock Entity::timer;
int UUID::nextUuid = 1;

Entity::Entity(string nm, EntityPosition pos, string gfx1, string gfx2) {
	graphics.load(gfx1,gfx2);
	position = pos;
	name = nm;
	position.dir = EntityPosition::Up;
	uuid = UUID::create();
	lTime = Entity::timer.getElapsedTime().asSeconds();
	speed[0] = 64;
	speed[1] = 128;
	boundingBox = FloatRect(0, 0, graphics.getSize().x, graphics.getSize().y);
	interactDistance = 12;
	collisionsEnabled = true;
}

Entity::~Entity() {
    cout << "Entity '" << name << "'(uuid: " << uuid << ") deleted\n";
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

FloatRect Entity::getInteractBox() {
    FloatRect box = boundingBox;
    box.left += position.coords.x;
    box.top += position.coords.y;

    switch (position.dir) {

    case EntityPosition::Up:
        box.height = interactDistance;
        box.top -= interactDistance;
        break;

    case EntityPosition::Right:
        box.width = interactDistance;
        box.left += boundingBox.width;
        break;

    case EntityPosition::Down:
        box.height = interactDistance;
        box.top += boundingBox.height;
        break;

    case EntityPosition::Left:
        box.width = interactDistance;
        box.left -= interactDistance;
        break;

    default:
        cout << "Warning: " << getIdString() << " has invalid dir " << position.dir << endl;
    }
    return box;
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

void Entity::notifyAttacked(Ptr attacker, const CombatAttack& attack) {
    cout << getIdString() << " ATTACKED BY " << attacker->getIdString() << endl;
    //TODO - apply damage and effects
}

void Entity::notifyInteracted(Ptr user) {
    cout << getIdString() << " INTERACTED BY " << user->getIdString() << endl;
}

void Entity::notifyCombatNearby(List combatants) {
    cout << getIdString() << " notified of nearby combat\n";
}

string Entity::getIdString() {
    stringstream ss;
    ss << "<" << getType() << ": '" << getName() << "'(uuid: " << uuid << ")>";
    return ss.str();
}

void Entity::render(sf::RenderTarget& target, sf::Vector2f camPos) {
	graphics.render(target,position.coords-camPos);
	bubble.render(target,position.coords-camPos);
}

bool Entity::move(EntityPosition::Direction dir, bool fast, float elapsedTime) {
	EntityPosition newPos = position;
	position.dir = dir;

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
		newPosOffset.coords += Vector2f(boundingBox.left, boundingBox.top);
		Vector2f size = Vector2f(boundingBox.width, boundingBox.height);
		if (EntityManager::get()->canMove(this, oldPosOffset, newPosOffset, size)) {
			graphics.setMoving(position.dir, fast);
			EntityPosition oldPos = position;
			FloatRect oldBox = getBoundingBox();
			position = newPos;
			EntityManager::get()->updatePosition(this, oldPos);
			MapManager::get()->registerEntityMovement(EntityManager::get()->getEntityPtr(this), oldBox);
			return true;
		}
	}
	return false;
}

void Entity::update() {
	lTime = Entity::timer.getElapsedTime().asSeconds();
}

Entity::Ptr Entity::interact(bool notify) {
    return EntityManager::get()->doInteract(this, notify);
}

bool Entity::collidesWithOtherEntities() {
    return collisionsEnabled;
}
