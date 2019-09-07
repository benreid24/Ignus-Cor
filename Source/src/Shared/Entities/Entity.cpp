#include "Shared/Entities/Entity.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Entities/EntityBehavior.hpp"
#include "Shared/Maps/MapManager.hpp"
#include "Shared/Util/UUID.hpp"
#include "Shared/DebugOverlays.hpp"
#include "Shared/Util/Timer.hpp"
#include <sstream>
using namespace std;
using namespace sf;

int UUID::nextUuid = 1;

EntityPosition::EntityPosition(Vector2f pos, Direction d) {
    coords = pos;
    dir = d;
    mapName = MapManager::get()->getCurrentMap();
}

Entity::Entity(string nm, EntityPosition pos, string gfx1, string gfx2) {
	graphics.load(gfx1,gfx2);
	position = pos;
	name = nm;
	uuid = UUID::create();
	timeLastUpdated = Timer::get().timeElapsedSeconds();
	speed[0] = 64;
	speed[1] = 128;
	boundingBox = FloatRect(0, 0, graphics.getSize().x, graphics.getSize().y);
	interactDistance = 12;
	collisionsEnabled = true;
	isYSorted = true;
	if (DebugOverlays::isOverlayActive(DebugOverlays::EntityInfo))
        cout << "Entity '" << name << "'(uuid: " << uuid << ") created\n";
}

Entity::~Entity() {
    if (DebugOverlays::isOverlayActive(DebugOverlays::EntityInfo))
        cout << "Entity '" << name << "'(uuid: " << uuid << ") deleted\n";
}

EntityPosition Entity::getPosition() const {
	return position;
}

FloatRect Entity::getBoundingBox() const {
	return FloatRect(
                  position.coords.x+boundingBox.left,
                  position.coords.y+boundingBox.top,
                  boundingBox.width,
                  boundingBox.height);
}

Vector2f Entity::getCenter() const {
    FloatRect box = getBoundingBox();
    return Vector2f(box.left+box.width/2, box.top+box.height/2);
}

FloatRect Entity::getInteractBox() const {
    FloatRect box = FloatRect(position.coords, graphics.getSize());

    switch (position.dir) {

    case EntityPosition::Up:
        box.height = interactDistance;
        box.top -= interactDistance;
        break;

    case EntityPosition::Right:
        box.left += box.width;
        box.width = interactDistance;
        break;

    case EntityPosition::Down:
        box.top += box.height;
        box.height = interactDistance;
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

void Entity::shift(sf::Vector2f amount, const string& newMap) {
    EntityPosition oldPos = position;
	position.coords += amount;
	if (newMap.size() > 0)
        position.mapName = newMap;
	EntityManager::get()->updatePosition(this, oldPos);
}

const string& Entity::getName() const {
	return name;
}

void Entity::notifyAttacked(Ptr attacker, CombatAttack::ConstPtr attack) {
    if (DebugOverlays::isOverlayActive(DebugOverlays::EntityInfo))
        cout << getIdString() << " ATTACKED BY " << attacker->getIdString() << endl;
    p_notifyAttacked(attacker, attack);
}

void Entity::notifyInteracted(Ptr user) {
    if (DebugOverlays::isOverlayActive(DebugOverlays::EntityInfo))
        cout << getIdString() << " INTERACTED BY " << user->getIdString() << endl;
    p_notifyInteracted(user);
}

void Entity::notifyCombatNearby(List combatants) {
    if (DebugOverlays::isOverlayActive(DebugOverlays::EntityInfo))
        cout << getIdString() << " notified of nearby combat\n";
    p_notifyCombatNearby(combatants);
}

string Entity::getIdString() const {
    stringstream ss;
    ss << "<" << getType() << ": '" << getName() << "'(uuid: " << uuid << ")>";
    return ss.str();
}

void Entity::render(sf::RenderTarget& target, sf::Vector2f camPos) {
	p_renderBelow(target, camPos);
	graphics.render(target,position.coords-camPos);
	bubble.render(target,position.coords-camPos);
	p_renderAbove(target, camPos);

	if (DebugOverlays::isOverlayActive(DebugOverlays::BoundingBoxes)) {
        RectangleShape bounds;
        bounds.setSize(Vector2f(getBoundingBox().width, getBoundingBox().height));
        bounds.setPosition(getBoundingBox().left-camPos.x, getBoundingBox().top-camPos.y);
        bounds.setFillColor(Color::Transparent);
        bounds.setOutlineColor(Color::Red);
        bounds.setOutlineThickness(2.5);

        RectangleShape intBounds;
        intBounds.setSize(Vector2f(getInteractBox().width, getInteractBox().height));
        intBounds.setPosition(getInteractBox().left-camPos.x, getInteractBox().top-camPos.y);
        intBounds.setFillColor(Color::Transparent);
        intBounds.setOutlineColor(Color::Cyan);
        intBounds.setOutlineThickness(1.5);

        target.draw(bounds);
        target.draw(intBounds);
	}
}

bool Entity::move(EntityPosition::Direction dir, bool fast, float elapsedTime) {
	Vector2f newPos = position.coords;
	position.dir = dir;

	elapsedTime = (elapsedTime==0)?(timeSinceLastUpdate()):(elapsedTime);
	int i = int(fast);
	float dist = speed[i]*elapsedTime;

	switch (position.dir) {
	case EntityPosition::Up:
        newPos.y -= dist;
        break;
	case EntityPosition::Right:
		newPos.x += dist;
		break;
	case EntityPosition::Down:
		newPos.y += dist;
		break;
	case EntityPosition::Left:
		newPos.x -= dist;
		break;
	default:
		cout << "Warning: Entity::move received an invalid direction!\n";
	}

	if (newPos != position.coords) {
		EntityPosition oldPosOffset = position;
		EntityPosition newPosOffset = EntityPosition(newPos, position.mapName, position.dir);
		oldPosOffset.coords += Vector2f(boundingBox.left, boundingBox.top);
		newPosOffset.coords += Vector2f(boundingBox.left, boundingBox.top);
		Vector2f size = Vector2f(boundingBox.width, boundingBox.height);

		if (EntityManager::get()->canMove(this, oldPosOffset, newPosOffset, size)) {
			graphics.setMoving(position.dir, fast);

			EntityPosition oldPos = position;
			FloatRect oldBox = getBoundingBox();
			position.coords = newPos;

			EntityManager::get()->updatePosition(this, oldPos);
			MapManager::get()->registerEntityMovement(EntityManager::get()->getEntityPtr(this), oldBox);
			return true;
		}
	}

	graphics.setDirection(position.dir);
	return false;
}

void Entity::update() {
    beforeTimerUpdate();
	timeLastUpdated = Timer::get().timeElapsedSeconds();
	afterTimerUpdate();
	graphics.update();
	bubble.update();
}

Entity::Ptr Entity::interact() {
    List ents = EntityManager::get()->getEntitiesInSpace(position.mapName, getInteractBox());
    Ptr me = EntityManager::get()->getEntityPtr(this);
    if (ents.size()>0) {
        if (ents.size()>1)
            cout << "Warning: " << getIdString() << " interacted with " << ents.size() << "entities\n";
        (*ents.begin())->notifyInteracted(me);
        return *ents.begin();
    }
    return Ptr(nullptr);
}

bool Entity::collidesWithOtherEntities() const {
    return collisionsEnabled;
}

bool Entity::isYSortRendered() const {
    return isYSorted;
}

EntityBubble& Entity::getBubble() {
    return bubble;
}

double Entity::timeSinceLastUpdate() const {
    return Timer::get().timeElapsedSeconds() - timeLastUpdated;
}
