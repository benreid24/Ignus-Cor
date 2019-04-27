#include "Shared/Entities/Virtual/AttackEntity.hpp"
using namespace std;

AttackEntity::AttackEntity(Entity::Ptr atk, const string& anim)
: Entity("set me", atk->getPosition(), anim, "") {
    attacker = atk;
    collisionsEnabled = false;
    //TODO - set position based on attacker bounding box and direction
}

const string AttackEntity::getType() {
    return "AttackEntity";
}

void AttackEntity::update() {
    Entity::update();
}
