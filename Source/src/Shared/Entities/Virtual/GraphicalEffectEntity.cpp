#include "Shared/Entities/Virtual/GraphicalEffectEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
using namespace std;

GraphicalEffectEntity::GraphicalEffectEntity(Entity::Ptr bindTo, EntityPosition pos, string gfx1)
: Entity("graphical effect", pos, gfx1) {
    boundTo = bindTo;
    wasBound = bindTo.get() != nullptr;
    collisionsEnabled = false;

    if (wasBound)
        offset = position.coords - bindTo->getPosition().coords;
}

void GraphicalEffectEntity::update() {
    Entity::update();

    if (!boundTo.expired()) {
        sf::Vector2f shiftAmt = position.coords - (boundTo.lock()->getPosition().coords + offset);
        Entity::shift(shiftAmt, boundTo.lock()->getPosition().mapName);
    }
    else if (wasBound)
        EntityManager::get()->remove(this);
}

void GraphicalEffectEntity::render(sf::RenderTarget& target, sf::Vector2f camPos) {
    //
}
