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
}

EntityPosition Entity::getPosition() {
	return position;
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
	//TODO - honor collisions (through EntityManager)
	EntityPosition oldPos = position;
	position.dir = d;
	graphics.setMoving(position.dir, fast);

	elapsedTime = (elapsedTime==0)?(Entity::timer.getElapsedTime().asSeconds()-lTime):(elapsedTime);
	int i = int(fast);
	float dist = speed[i]*elapsedTime;

	switch (position.dir) {
	case 0:
        position.coords.y -= dist;
        Entity::entityManager->updatePosition(this, oldPos);
        break;
	case 1:
		position.coords.x += dist;
		Entity::entityManager->updatePosition(this, oldPos);
		break;
	case 2:
		position.coords.y += dist;
		Entity::entityManager->updatePosition(this, oldPos);
		break;
	case 3:
		position.coords.x -= dist;
		Entity::entityManager->updatePosition(this, oldPos);
		break;
	default:
		cout << "Warning: Entity::move received an invalid direction!\n";
	}
}

void Entity::update() {
	lTime = Entity::timer.getElapsedTime().asSeconds();
}

void Entity::setEntityManager(EntityManager* em) {
	entityManager = em;
}
