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

void GraphicalEffectEntity::afterTimerUpdate() {
    if (!boundTo.expired()) {
        sf::Vector2f shiftAmt = (boundTo.lock()->getPosition().coords + offset) - position.coords;
        Entity::shift(shiftAmt, boundTo.lock()->getPosition().mapName);
    }
    else if (wasBound && removeWithBound && canDestroy())
        EntityManager::get()->remove(this);
}

void GraphicalEffectEntity::render(sf::RenderTarget& target, sf::Vector2f camPos) {
    Entity::render(target, camPos);
}

bool GraphicalEffectEntity::canDestroy() {
    return true;
}
