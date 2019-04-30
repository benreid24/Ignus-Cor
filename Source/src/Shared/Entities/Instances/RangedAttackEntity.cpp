#include "Shared/Entities/Instances/RangedAttackEntity.hpp"
#include "Shared/Entities/Instances/MeleeAttackEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
using namespace std;

RangedAttackEntity::RangedAttackEntity(Entity::Ptr atk, const CombatRangedAttack& weapon)
: AttackEntity(atk, weapon.getName(), weapon.getAnimation()), attack(weapon) {
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
        for (Entity::List::iterator i = hits.begin(); i!=hits.end(); ++i) {
            (*i)->notifyAttacked(attacker, attack);
        }

        if (attack.getImpactAnimation().size() > 0) {
            Entity::Ptr explosion = MeleeAttackEntity::create(attacker, attack.toExplosionAttack());
            explosion->setPositionAndDirection(position);
            EntityManager::get()->add(explosion);
        }
        EntityManager::get()->remove(this);
    }
}
