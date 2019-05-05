#include "Shared/Entities/Instances/RangedAttackEntity.hpp"
#include "Shared/Entities/Instances/DirectAttackEntity.hpp"
#include "Shared/Entities/Instances/ParticleGeneratorEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Maps/MapManager.hpp"
#include <cmath>
using namespace std;

RangedAttackEntity::RangedAttackEntity(Entity::Ptr atk, const CombatAttack& weapon, int atkDir)
: AttackEntity(atk, weapon) {
    position = atk->getPosition();
    position.coords.x += atk->getBoundingBox().width/2 - boundingBox.width/2;
    position.coords.y += atk->getBoundingBox().height/2 - boundingBox.height/2;
    speed[0] = attack.getSpeed();
    float dirRadians = float(atkDir) / 180 * 3.1415926;
    cosDir = cos(dirRadians);
    sinDir = sin(dirRadians);
    distanceTraveled = 0;
}

Entity::Ptr RangedAttackEntity::create(Entity::Ptr attacker, const CombatAttack& atk, int atkDir) {
    return Entity::Ptr(new RangedAttackEntity(attacker, atk, atkDir));
}

const string RangedAttackEntity::getType() {
    return "RangedAttackEntity";
}

void RangedAttackEntity::update() {
    bool done = false;

    EntityPosition oldPos = position;
    float displacement = speed[0] * (Entity::timer.getElapsedTime().asSeconds()-lTime);
    sf::Vector2f movedist(displacement*cosDir, displacement*sinDir);
    Entity::shift(movedist);
    distanceTraveled += displacement;

    Entity::List hits = EntityManager::get()->getEntitiesInSpace(position.mapName, getBoundingBox());
    if ((hits.size()>1 && find(hits.begin(), hits.end(), attacker)==hits.end()) || hits.size()>2) {
        //apply damage
        for (Entity::List::iterator i = hits.begin(); i!=hits.end(); ++i) {
            if (shouldApplyDamage(*i)) {
                (*i)->notifyAttacked(attacker, attack);
            }
        }
        done = true;
    }
    if (distanceTraveled >= attack.getRange() || !MapManager::get()->spaceFree(position, sf::Vector2f(boundingBox.width, boundingBox.height)))
        done = true;

    if (done) {
        //create explosion
        if (attack.getImpactAnimation().size() > 0) {
            Entity::Ptr explosion = DirectAttackEntity::create(attacker, attack.toExplosionAttack(), false);
            EntityPosition pos = position;

            sf::FloatRect xbox = explosion->getBoundingBox();
            displacement = boundingBox.width/8 + boundingBox.height/8;
            pos.coords.x += boundingBox.width/2 - xbox.width/2 + cosDir*displacement;
            pos.coords.y += boundingBox.height/2 - xbox.height/2 + sinDir*displacement;
            explosion->setPositionAndDirection(pos);

            EntityManager::get()->add(explosion);
        }
        EntityManager::get()->remove(this);
    }

    AttackEntity::update();
}
