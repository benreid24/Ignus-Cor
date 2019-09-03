#include "Shared/Entities/Virtual/AiEntity.hpp"
#include "Shared/Entities/EntityBehavior.hpp"
using namespace std;

AiEntity::AiEntity(string nm, EntityPosition pos, string gfx1, string gfx2)
: CombatEntity(nm, pos, gfx1, gfx2), conversation(this) {
    behavior = nullptr;
}

AiEntity::~AiEntity() {
    if (behavior != nullptr)
        delete behavior; //TODO - either make the base responsible for allocation or make the children responsible for deletion
}

void AiEntity::beforeTimerUpdate() {
    beforeBehaviorUpdate();
    conversation.update();
    if (behavior != nullptr)
        behavior->update();
    afterBehaviorUpdate();
}

void AiEntity::notifyCombatAttacked(Ptr attacker, CombatAttack::ConstPtr attack) {
    if (behavior != nullptr)
        behavior->notifyAttacked(attacker, attack);
    notifyAiAttacked(attacker, attack);
}

void AiEntity::notifyInteractedCustom(Ptr user) {
    if (behavior != nullptr)
        behavior->notifyInteracted(user);
    notifyAiInteracted(user);
}

void AiEntity::notifyCombatNearbyCustom(List combatants) {
    if (behavior != nullptr)
        behavior->notifyCombatNearby(combatants);
    notifyAiCombatNearby(combatants);
}
