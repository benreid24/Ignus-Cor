#include "Shared/Entities/Virtual/AiEntity.hpp"
#include "Shared/Entities/EntityBehavior.hpp"

#ifdef GAME
#include <Game/Core/Game.hpp>
#endif // GAME
using namespace std;

namespace {
Entity* getPlayer(Entity* fallback) {
    #ifdef GAME
    return Game::get().player.get();
    #else
    return fallback;
    #endif // GAME
}
}

AiEntity::AiEntity(string nm, EntityPosition pos, string gfx1, string gfx2)
: CombatEntity(nm, pos, gfx1, gfx2), conversation(getPlayer(this), this) {
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

void AiEntity::p_notifyWasAttacked(Ptr attacker, CombatAttack::Ref attack) {
    if (behavior != nullptr)
        behavior->notifyAttacked(attacker, attack);
    p_notifyAiAttacked(attacker, attack);
}

void AiEntity::p_notifyInteracted(Ptr user) {
    if (behavior != nullptr)
        behavior->notifyInteracted(user);
    p_notifyAiInteracted(user);
}

void AiEntity::p_notifyCombatNearby(List combatants) {
    if (behavior != nullptr)
        behavior->notifyCombatNearby(combatants);
    p_notifyAiCombatNearby(combatants);
}
