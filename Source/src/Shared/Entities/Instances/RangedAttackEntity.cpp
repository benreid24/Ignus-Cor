#include "Shared/Entities/Instances/RangedAttackEntity.hpp"
#include "Shared/Entities/Instances/DirectAttackEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include <cmath>
using namespace std;

RangedAttackEntity::RangedAttackEntity(Entity::Ptr atk, const CombatAttack& weapon, int atkDir)
: AttackEntity(atk, weapon.getName(), weapon.getAnimation()), attack(weapon) {
    speed[0] = attack.getSpeed();
    collisionsEnabled = true;
    float dirRadians = float(atkDir) / 180 * 3.1415926;
    cosDir = cos(dirRadians);
    sinDir = sin(dirRadians);
}

Entity::Ptr RangedAttackEntity::create(Entity::Ptr attacker, const CombatAttack& atk, int atkDir) {
    return Entity::Ptr(new RangedAttackEntity(attacker, atk, atkDir));
}

const string RangedAttackEntity::getType() {
    return "RangedAttackEntity";
}

void RangedAttackEntity::update() {
    EntityPosition oldPos = position;
    float displacement = speed[0] * (Entity::timer.getElapsedTime().asSeconds()-lTime);
    sf::Vector2f movedist(displacement*cosDir, displacement*sinDir);
    Entity::shift(movedist, false);

    if (!EntityManager::get()->canMove(this, oldPos, position, sf::Vector2f(boundingBox.width,boundingBox.height))) {
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
