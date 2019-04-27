#include "Shared/Entities/Instances/RangedAttackEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
using namespace std;

RangedAttackEntity::RangedAttackEntity(Entity::Ptr atk)
: AttackEntity(atk) {
    //TODO - set damage, speed, graphics etc from weapon
    speed[0] = 512;
    interactDistance = 12; //larger radius attacks?
}

const string RangedAttackEntity::getType() {
    return "RangedAttackEntity";
}

void RangedAttackEntity::update() {
    Entity::update();
    if (!move(position.dir, false)) {
        Entity::Ptr target = interact(false);
        if (target) {
            //TODO - apply damage
        }
        EntityManager::get()->remove(EntityManager::get()->getEntityPtr(this));
    }
}
