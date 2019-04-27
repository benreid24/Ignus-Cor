#include "Shared/Entities/Instances/RangedAttackEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
using namespace std;

RangedAttackEntity::RangedAttackEntity(Entity::Ptr atk, const CombatRangedAttack& weapon)
: AttackEntity(atk, weapon.getAnimation()), attack(weapon) {
    speed[0] = attack.getSpeed();
    position = attacker->getPosition();
}

Entity::Ptr RangedAttackEntity::create(Entity::Ptr attacker, const CombatRangedAttack& atk) {
    return Entity::Ptr(new RangedAttackEntity(attacker, atk));
}

const string RangedAttackEntity::getType() {
    return "RangedAttackEntity";
}

void RangedAttackEntity::update() {
    AttackEntity::update();
    if (!move(position.dir, false)) {
        Entity::List hits = EntityManager::get()->getEntitiesInSpace(position.mapName, boundingBox);
        //TODO - apply damage. check for explosion. create explosion entity. harm caster?
        EntityManager::get()->remove(EntityManager::get()->getEntityPtr(this));
    }
}
