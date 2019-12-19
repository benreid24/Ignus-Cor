#include "Shared/Entities/Virtual/AttackEntity.hpp"
#include "Shared/Entities/Instances/RangedAttackEntity.hpp"
#include "Shared/Entities/Instances/DirectAttackEntity.hpp"
#include "Shared/Entities/Instances/ParticleGeneratorEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
using namespace std;

AttackEntity::AttackEntity(Entity::Ptr atk, CombatAttack::Ref weapon)
: Entity(weapon->getName(), atk->getPosition(), weapon->getMapGfxFile(), "") {
    attacker = atk;
    attack = weapon;
    particlesCreated = false;

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
            cout << "Warning: Attack '" << weapon->getName() << "' created with invalid direction\n";
    }
    position.coords = sf::Vector2f(x,y);
}

const string AttackEntity::getType() const {
    return "AttackEntity";
}

void AttackEntity::afterTimerUpdate() {
    if (!particlesCreated) {
        particlesCreated = true;
        ParticleGenerator::Ptr gen = ParticleGeneratorFactory::create(attack->getParticleType(),
                                                                      ParticleGenerator::UntilDestroyedLifetime,
                                                                      attack->getParticlePersistanceTime());
        EntityPosition pos = position;
        pos.coords = getCenter();
        Entity::Ptr genEnt = ParticleGeneratorEntity::create(EntityManager::get().getEntityPtr(this), pos, gen);
        EntityManager::get().add(genEnt);
    }
    p_afterTimerUpdate();
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

Entity::Ptr AttackEntity::create(Entity::Ptr attacker, CombatAttack::Ref atk, int atkDir) {
    switch (atk->getType()) {
        case CombatAttack::Ranged:
            return RangedAttackEntity::create(attacker, atk, atkDir);
        case CombatAttack::Melee:
            return DirectAttackEntity::create(attacker, atk);

        default:
            cout << "Error: Invalid CombatAttack type " << atk->getType() << " from " << attacker->getIdString() << endl;
            return Entity::Ptr(nullptr);
    }
}
