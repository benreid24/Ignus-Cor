#include "Shared/Entities/Virtual/AiEntity.hpp"
#include "Shared/Entities/EntityBehavior.hpp"
using namespace std;

AiEntity::AiEntity(string nm, EntityPosition pos, string gfx1, string gfx2) : CombatEntity(nm, pos, gfx1, gfx2) {
    behavior = nullptr;
}

AiEntity::~AiEntity() {
    if (behavior != nullptr)
        delete behavior;
}

void AiEntity::update() {
    CombatEntity::update();
    if (behavior != nullptr)
        behavior->update();
}

void AiEntity::notifyAttacked(Ptr attacker, const CombatAttack& attack) {
    CombatEntity::notifyAttacked(attacker, attack);
    if (behavior != nullptr)
        behavior->notifyAttacked(attacker, attack);
}

void AiEntity::notifyInteracted(Ptr user) {
    CombatEntity::notifyInteracted(user);
    if (behavior != nullptr)
        behavior->notifyInteracted(user);
}

void AiEntity::notifyCombatNearby(List combatants) {
    CombatEntity::notifyCombatNearby(combatants);
    if (behavior != nullptr)
        behavior->notifyCombatNearby(combatants);
}
