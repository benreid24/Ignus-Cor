#include "Shared/Entities/Virtual/AttackEntity.hpp"
#include "Shared/Entities/Instances/RangedAttackEntity.hpp"
#include "Shared/Entities/Instances/MeleeAttackEntity.hpp"
using namespace std;

AttackEntity::AttackEntity(Entity::Ptr atk, const string& anim)
: Entity("set me", atk->getPosition(), anim, "") {
    attacker = atk;
    collisionsEnabled = false;
    //TODO - set position based on attacker bounding box and direction
}

const string AttackEntity::getType() {
    return "AttackEntity";
}

void AttackEntity::update() {
    Entity::update();
}

Entity::Ptr AttackEntity::create(Entity::Ptr attacker, const CombatAttack& atk) {
    try {
        const CombatRangedAttack& rangedAttack = dynamic_cast<const CombatRangedAttack&>(atk);
        return RangedAttackEntity::create(attacker, rangedAttack);
    }
    catch (bad_cast& exc) {
        return MeleeAttackEntity::create(attacker, atk);
    }
}
