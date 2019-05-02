#include "Shared/Entities/Virtual/AttackEntity.hpp"
#include "Shared/Entities/Instances/RangedAttackEntity.hpp"
#include "Shared/Entities/Instances/MeleeAttackEntity.hpp"
using namespace std;

AttackEntity::AttackEntity(Entity::Ptr atk, const string& atkNm, const string& anim)
: Entity(atkNm, atk->getPosition(), anim, "") {
    attacker = atk;

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

bool AttackEntity::shouldApplyDamage(Entity::Ptr ent) {
    if (find(entitiesHit.begin(), entitiesHit.end(), ent) == entitiesHit.end()) {
        if (ent.get() != attacker.get() && ent.get() != this) {
            entitiesHit.push_back(ent);
            return true;
        }
    }
    return false;
}

Entity::Ptr AttackEntity::create(Entity::Ptr attacker, const CombatAttack& atk) {
    switch (atk.getType()) {
        case CombatAttack::Ranged:
            return RangedAttackEntity::create(attacker, atk);
        case CombatAttack::Melee:
            return MeleeAttackEntity::create(attacker, atk);

        default:
            cout << "Error: Invalid CombatAttack type " << atk.getType() << " from " << attacker->getIdString() << endl;
            return Entity::Ptr(nullptr);
    }
}
