#include "Shared/Entities/Instances/CreatureEntity.hpp"
using namespace std;

CreatureEntity::CreatureEntity(string nm, EntityPosition pos, string gfx1, string gfx2, File& file) : AiEntity(nm, pos, gfx1, gfx2) {
    //TODO - load from file/spawner
}

Entity::Ptr CreatureEntity::create(string dataFile) {
    File file(dataFile, File::In);
    return Entity::Ptr(new CreatureEntity("dummy", EntityPosition(), "", "", file)); //TODO - load data from file
}

const string CreatureEntity::getType() {
    return "CreatureEntity";
}

void CreatureEntity::update() {
    AiEntity::update();
}

void CreatureEntity::notifyAttacked(Entity::Ptr attacker) {
    AiEntity::notifyAttacked(attacker);
}

void CreatureEntity::notifyInteracted(Entity::Ptr user) {
    AiEntity::notifyInteracted(user);
}

void CreatureEntity::notifyCombatNearby(Entity::List combatants) {
    AiEntity::notifyCombatNearby(combatants);
}

