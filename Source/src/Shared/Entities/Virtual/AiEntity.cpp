#include "Shared/Entities/Virtual/AiEntity.hpp"
#include "Shared/Entities/EntityBehavior.hpp"
using namespace std;

AiEntity::AiEntity(string nm, EntityPosition pos, string gfx1, string gfx2) : Entity(nm, pos, gfx1, gfx2) {
    behavior = nullptr;
    stats.canDie = true;
}

AiEntity::~AiEntity() {
    if (behavior != nullptr)
        delete behavior;
}

const string AiEntity::getType() {
    return "AiEntity";
}

void AiEntity::update() {
    Entity::update();
    if (behavior != nullptr)
        behavior->update();
}

void AiEntity::notifyAttacked(Ptr attacker, const CombatAttack& attack) {
    Entity::notifyAttacked(attacker, attack);
    if (behavior != nullptr)
        behavior->notifyAttacked(attacker, attack);
}

void AiEntity::notifyInteracted(Ptr user) {
    Entity::notifyInteracted(user);
    if (behavior != nullptr)
        behavior->notifyInteracted(user);
}

void AiEntity::notifyCombatNearby(List combatants) {
    Entity::notifyCombatNearby(combatants);
    if (behavior != nullptr)
        behavior->notifyCombatNearby(combatants);
}
