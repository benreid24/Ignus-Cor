#include "Shared/Entities/Instances/NpcEntity.hpp"
#include "Shared/Entities/Behaviors/WanderingBehavior.hpp"
using namespace std;

NpcEntity::NpcEntity(string nm, EntityPosition pos, string gfx1, string gfx2) : AiEntity(nm, pos, gfx1, gfx2) {
    //TODO - add other data like conversation
    behavior = new WanderingBehavior(this);
}

Entity::Ptr NpcEntity::create(string dataFile) {
    File file(dataFile, File::In);
    return Entity::Ptr(new NpcEntity("dummy", EntityPosition(), "", ""));
}

Entity::Ptr NpcEntity::create(string nm, EntityPosition pos, string gfx1, string gfx2) {
    return Entity::Ptr(new NpcEntity(nm, pos, gfx1, gfx2));
}

const string NpcEntity::getType() const {
    return "NpcEntity";
}

void NpcEntity::p_notifyAiInteracted(Entity::Ptr user) {
    conversation.activate();
}
