#include "Shared/Entities/Instances/NpcEntity.hpp"
using namespace std;

NpcEntity::NpcEntity(string nm, EntityPosition pos, string gfx1, string gfx2, File& file) : AiEntity(nm, pos, gfx1, gfx2) {
    //TODO - load from file
}

Entity::Ptr NpcEntity::create(string dataFile) {
    File file(dataFile, File::In);
    return Entity::Ptr(new NpcEntity("dummy", EntityPosition(), "", "", file)); //TODO - load data from file
}

const string NpcEntity::getType() {
    return "NpcEntity";
}

void NpcEntity::update() {
    AiEntity::update();
}

void NpcEntity::notifyAttacked(Entity::Ptr attacker, const CombatAttack& attack) {
    AiEntity::notifyAttacked(attacker, attack);
}

void NpcEntity::notifyInteracted(Entity::Ptr user) {
    AiEntity::notifyInteracted(user);
}

void NpcEntity::notifyCombatNearby(Entity::List combatants) {
    AiEntity::notifyCombatNearby(combatants);
}
