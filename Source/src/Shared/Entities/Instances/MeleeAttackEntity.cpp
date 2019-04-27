#include "Shared/Entities/Instances/MeleeAttackEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
using namespace std;

MeleeAttackEntity::MeleeAttackEntity(Entity::Ptr attacker)
: AttackEntity(attacker) {
    speed[0] = speed[1] = 0;
}

Entity::Ptr MeleeAttackEntity::create(Entity::Ptr attacker) {
    return Entity::Ptr(new MeleeAttackEntity(attacker));
}

const string MeleeAttackEntity::getType() {
    return "MeleeAttackEntity";
}

void MeleeAttackEntity::update() {
    Entity::List hits = EntityManager::get()->getEntitiesInSpace(position.mapName, boundingBox);
    for (Entity::List::iterator i = hits.begin(); i!=hits.end(); ++i) {
        if (find(entitiesHit.begin(), entitiesHit.end(), *i) == entitiesHit.end()) {
            entitiesHit.push_back(*i);
            //TODO - do weapon hit
        }
    }
}
