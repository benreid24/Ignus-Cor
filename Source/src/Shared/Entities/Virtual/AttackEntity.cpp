#include "Shared/Entities/Virtual/AttackEntity.hpp"
#include "Shared/Entities/Instances/RangedAttackEntity.hpp"
#include "Shared/Entities/Instances/MeleeAttackEntity.hpp"
using namespace std;

AttackEntity::AttackEntity(Entity::Ptr atk, const string& atkNm, const string& anim)
: Entity(atkNm, atk->getPosition(), anim, "") {
    attacker = atk;
    collisionsEnabled = false;

    sf::FloatRect atkrBox = attacker->getBoundingBox();
    sf::FloatRect atkBox = getBoundingBox();
    float x = -100, y = -100;

    switch (position.dir) {
        case EntityPosition::Up:
            x = atkrBox.left + atkrBox.width/2 - atkBox.width/2;
            y = atkrBox.top - atkBox.height;
            break;

        case EntityPosition::Right:
            x = atkrBox.left + atkrBox.width;
            y = atkrBox.top + atkrBox.height/2 - atkBox.height/2;
            break;

        case EntityPosition::Down:
            x = atkrBox.left + atkrBox.width/2 - atkBox.width/2;
            y = atkrBox.top + atkrBox.height;
            break;

        case EntityPosition::Left:
            x = atkrBox.left - atkBox.width;
            y = atkrBox.top + atkrBox.height/2 - atkBox.height/2;
            break;

        default:
            cout << "Warning: Attack '" << atkNm << "' created with invalid direction\n";
    }
    position.coords = sf::Vector2f(x,y);
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
