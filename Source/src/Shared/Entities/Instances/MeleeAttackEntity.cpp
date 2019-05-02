#include "Shared/Entities/Instances/MeleeAttackEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
using namespace std;

MeleeAttackEntity::MeleeAttackEntity(Entity::Ptr attacker, const CombatAttack& atk)
: AttackEntity(attacker, atk.getName(), atk.getAnimation()), attack(atk) {
    speed[0] = speed[1] = 0;
}

Entity::Ptr MeleeAttackEntity::create(Entity::Ptr attacker, const CombatAttack& atk) {
    return Entity::Ptr(new MeleeAttackEntity(attacker, atk));
}

const string MeleeAttackEntity::getType() {
    return "MeleeAttackEntity";
}

void MeleeAttackEntity::update() {
    Entity::List hits = EntityManager::get()->getEntitiesInSpace(position.mapName, getBoundingBox());
    for (Entity::List::iterator i = hits.begin(); i!=hits.end(); ++i) {
        if (find(entitiesHit.begin(), entitiesHit.end(), *i) == entitiesHit.end()) {
            if (i->get() != attacker.get() && i->get() != this) {
                entitiesHit.push_back(*i);
                (*i)->notifyAttacked(attacker, attack);
            }
        }
    }

    AttackEntity::update();
    if (graphics.animationDone())
        EntityManager::get()->remove(this);

    graphics.setMoving(attacker->getPosition().dir, false);
}
