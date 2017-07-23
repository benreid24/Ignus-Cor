#include "Shared/Entities/Entity.hpp"
#include "Shared/Util/UUID.hpp"
using namespace std;
using namespace sf;

Clock Entity::timer;
int UUID::nextUuid = 1;

Entity::Entity(string nm, Vector2f pos, string gfx1, string gfx2) {
	graphics.load(gfx1,gfx2);
	position = pos;
	name = nm;
	dir = 0;
	uuid = UUID::create();
	lTime = Entity::timer.getElapsedTime().asSeconds();
}

Vector2f Entity::getPosition() {
	return position;
}

void Entity::setPositionAndDirection(sf::Vector2f pos, int d) {
	position = pos;
	dir = d;
}

void Entity::shift(sf::Vector2f amount, bool truncate) {
	position += amount;
	if (truncate) {
        position.x = int(position.x/32)*32;
        position.y = int(position.y/32)*32;
	}
}

int Entity::getDir() {
	return dir;
}

string Entity::getName() {
	return name;
}

void Entity::render(sf::RenderTarget& target, sf::Vector2f camPos) {
	graphics.render(target,position-camPos);
	bubble.render(target,position-camPos);
}

void Entity::move(int d, bool fast, float elapsedTime) {
	//TODO - call update position in EntityManager
	dir = d;
	int i = int(fast);
	elapsedTime = (elapsedTime==0)?(Entity::timer.getElapsedTime().asSeconds()-lTime):(elapsedTime);
	float dist = speed[i]*elapsedTime;
	switch (dir) {
	case 0:
        position.y -= dist;
        break;
	case 1:
		position.x += dist;
		break;
	case 2:
		position.y += dist;
		break;
	case 3:
		position.x -= dist;
		break;
	default:
		cout << "Invalid direction!\n";
	}
}

void Entity::update() {
	lTime = Entity::timer.getElapsedTime().asSeconds();
}
