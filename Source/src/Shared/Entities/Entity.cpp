#include "Shared/Entities/Entity.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Util/UUID.hpp"
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
	boxTopOffset = 12;
}

EntityPosition Entity::getPosition() {
	return position;
}

FloatRect Entity::getBoundingBox() {
	return FloatRect(position.coords.x, position.coords.y+boxTopOffset, graphics.getSize().x, graphics.getSize().y-boxTopOffset);
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
		oldPosOffset.coords.y += boxTopOffset;
		newPosOffset.coords.y += boxTopOffset;
		Vector2f size = Vector2f(graphics.getSize().x, graphics.getSize().y-boxTopOffset);
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
