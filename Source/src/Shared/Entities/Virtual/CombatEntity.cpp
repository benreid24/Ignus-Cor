#include "Shared/Entities/Virtual/CombatEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include <iostream>
using namespace std;

CombatEntity::CombatEntity(string nm, EntityPosition pos, string gfx1, string gfx2)
: Entity(nm, pos, gfx1, gfx2) {
    xpRewardMultiplier = 1;
}

void CombatEntity::update() {
    Entity::update();
}

void CombatEntity::notifyAttacked(Entity::Ptr attacker, const CombatAttack& atk) {
    Entity::notifyAttacked(attacker, atk);

    CombatEntity* atkr = dynamic_cast<CombatEntity*>(attacker.get());
    if (atkr!=nullptr) {
        double ld = atkr->stats.level - stats.level;
        double lvlMult = (ld>0) ? (ld*ld) : (2.5/abs(ld));
        double atkPower = atk.getPower() * lvlMult;
        double damage = atkPower - armor.getDamageResist();
        stats.health -= damage;
        //TODO - effects and create particle generators from them

        if (stats.health<=0) {
            int levelDiff = stats.level - atkr->stats.level;
            double levelMult = (levelDiff>0) ? (levelDiff*levelDiff) : (0);
            double rewardXp = levelMult*xpRewardMultiplier + stats.level;
            atkr->stats.currentXp += rewardXp; //TODO - level up?

            EntityManager::get()->remove(EntityManager::get()->getEntityPtr(this));
        }
    }
    else
        cout << "Warning: " << getIdString() << " attacked by non CombatEntity " << attacker->getIdString() << endl;
}

void CombatEntity::notifyCombatNearby(Entity::List combatants) {
    Entity::notifyCombatNearby(combatants);
}

void CombatEntity::notifyInteracted(Entity::Ptr user) {
    Entity::notifyInteracted(user);
}

EntityStats& CombatEntity::getStats() {
    return stats;
}

const CombatArmor& CombatEntity::getArmor() {
    return armor;
}
