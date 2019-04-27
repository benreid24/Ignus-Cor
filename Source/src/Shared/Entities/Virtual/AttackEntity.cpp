#include "Shared/Entities/Virtual/AttackEntity.hpp"
using namespace std;

AttackEntity::AttackEntity(Entity::Ptr atk)
: Entity("set me", atk->getPosition(), "", "") {
    attacker = atk;
    collisionsEnabled = false;
}

const string AttackEntity::getType() {
    return "AttackEntity";
}
