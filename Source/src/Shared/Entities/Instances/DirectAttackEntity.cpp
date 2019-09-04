#include "Shared/Entities/Instances/DirectAttackEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
using namespace std;

DirectAttackEntity::DirectAttackEntity(Entity::Ptr attacker, CombatAttack::ConstPtr atk, bool followAttacker)
: AttackEntity(attacker, atk) {
    speed[0] = speed[1] = 0;
    offset = position.coords - attacker->getPosition().coords;
    follow = followAttacker;
    collisionsEnabled = false;
}

Entity::Ptr DirectAttackEntity::create(Entity::Ptr attacker, CombatAttack::ConstPtr atk, bool follow) {
    return Entity::Ptr(new DirectAttackEntity(attacker, atk, follow));
}

const string DirectAttackEntity::getType() const {
    return "DirectAttackEntity";
}

void DirectAttackEntity::beforeTimerUpdate() {
    if (follow) {
        sf::Vector2f shiftAmount = attacker->getPosition().coords + offset - position.coords;
        Entity::shift(shiftAmount);
    }

    Entity::List hits = EntityManager::get()->getEntitiesInSpace(position.mapName, getBoundingBox());
    for (Entity::List::iterator i = hits.begin(); i!=hits.end(); ++i) {
        if (shouldApplyDamage(*i)) {
            (*i)->notifyAttacked(attacker, attack);
        }
    }
}

void DirectAttackEntity::p_afterTimerUpdate() {
    if (graphics.animationDone())
        EntityManager::get()->remove(this);

    graphics.setMoving(position.dir, false);
}
