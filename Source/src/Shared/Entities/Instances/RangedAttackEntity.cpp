#include "Shared/Entities/Instances/RangedAttackEntity.hpp"
#include "Shared/Entities/Instances/DirectAttackEntity.hpp"
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
        //move over slightly more to get in bounding box
        sf::Vector2f shiftAmount;
        double shiftValue = speed[0] * (Entity::timer.getElapsedTime().asSeconds()-lTime);
        switch (position.dir) {
            case EntityPosition::Up:
                shiftAmount.y = -shiftValue;
                break;
            case EntityPosition::Right:
                shiftAmount.x = shiftValue;
                break;
            case EntityPosition::Down:
                shiftAmount.y = shiftValue;
                break;
            case EntityPosition::Left:
                shiftAmount.x = -shiftValue;
                break;
        }
        Entity::shift(shiftAmount, false);

        //apply damage
        Entity::List hits = EntityManager::get()->getEntitiesInSpace(position.mapName, getBoundingBox());
        for (Entity::List::iterator i = hits.begin(); i!=hits.end(); ++i) {
            if (shouldApplyDamage(*i)) {
                (*i)->notifyAttacked(attacker, attack);
            }
        }

        //create explosion
        if (attack.getImpactAnimation().size() > 0) {
            Entity::Ptr explosion = DirectAttackEntity::create(attacker, attack.toExplosionAttack(), false);

            EntityPosition pos = position;
            sf::FloatRect xbox = explosion->getBoundingBox();
            switch (position.dir) {
                case EntityPosition::Up:
                    pos.coords.y -= getBoundingBox().height/2;
                    pos.coords.x -= xbox.width/2 - getBoundingBox().width/2;
                    break;
                case EntityPosition::Right:
                    pos.coords.x += getBoundingBox().width/2;
                    pos.coords.y -= xbox.height/2 - getBoundingBox().height/2;
                    break;
                case EntityPosition::Down:
                    pos.coords.y += getBoundingBox().height/2;
                    pos.coords.x -= xbox.width/2 - getBoundingBox().width/2;
                    break;
                case EntityPosition::Left:
                    pos.coords.x -= getBoundingBox().width/2;
                    pos.coords.y -= xbox.height/2 - getBoundingBox().height/2;
                    break;
            }

            explosion->setPositionAndDirection(pos);
            EntityManager::get()->add(explosion);
        }
        EntityManager::get()->remove(this);
    }

    AttackEntity::update();
}
