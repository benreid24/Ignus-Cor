#include "Shared/Entities/Virtual/GraphicalEffectEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
using namespace std;

GraphicalEffectEntity::GraphicalEffectEntity(Entity::Ptr bindTo, EntityPosition pos, string gfx1, bool remWithBound)
: Entity("graphical effect", pos, gfx1) {
    boundTo = bindTo;
    wasBound = bindTo.get() != nullptr;
    collisionsEnabled = false;
    removeWithBound = remWithBound;

    if (wasBound)
        offset = position.coords - bindTo->getPosition().coords;
}

void GraphicalEffectEntity::update() {
    Entity::update();

    if (!boundTo.expired()) {
        sf::Vector2f shiftAmt = position.coords - (boundTo.lock()->getPosition().coords + offset);
        Entity::shift(shiftAmt, boundTo.lock()->getPosition().mapName);
    }
    else if (wasBound && removeWithBound)
        EntityManager::get()->remove(this);
}

void GraphicalEffectEntity::render(sf::RenderTarget& target, sf::Vector2f camPos) {
    Entity::render(target, camPos);
}

const string GraphicalEffectEntity::getType() {
    return "GraphicalEffectEntity";
}
