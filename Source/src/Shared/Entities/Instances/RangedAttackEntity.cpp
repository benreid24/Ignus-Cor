#include "Shared/Entities/Instances/RangedAttackEntity.hpp"
#include "Shared/Entities/Instances/MeleeAttackEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
using namespace std;

RangedAttackEntity::RangedAttackEntity(Entity::Ptr atk, const CombatAttack& weapon)
: AttackEntity(atk, weapon.getName(), weapon.getAnimation()), attack(weapon) {
    speed[0] = attack.getSpeed();
    collisionsEnabled = true;
}

Entity::Ptr RangedAttackEntity::create(Entity::Ptr attacker, const CombatAttack& atk) {
    return Entity::Ptr(new RangedAttackEntity(attacker, atk));
}

const string RangedAttackEntity::getType() {
    return "RangedAttackEntity";
}

void RangedAttackEntity::update() {
    if (!move(position.dir, false)) {
        Entity::List hits = EntityManager::get()->getEntitiesInSpace(position.mapName, boundingBox);
        for (Entity::List::iterator i = hits.begin(); i!=hits.end(); ++i) {
            if (shouldApplyDamage(*i)) {
                (*i)->notifyAttacked(attacker, attack);
            }
        }

        if (attack.getImpactAnimation().size() > 0) {
            Entity::Ptr explosion = MeleeAttackEntity::create(attacker, attack.toExplosionAttack(), false);
            explosion->setPositionAndDirection(position);
            EntityManager::get()->add(explosion);
        }
        EntityManager::get()->remove(this);
    }

    AttackEntity::update();
}
