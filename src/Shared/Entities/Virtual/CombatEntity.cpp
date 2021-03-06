#include "Shared/Entities/Virtual/CombatEntity.hpp"
#include "Shared/Entities/Virtual/AttackEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/DebugOverlays.hpp"
#include "Shared/Items/ItemDB.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

CombatEntity::CombatEntity(string nm, EntityPosition pos, string gfx1, string gfx2)
: Entity(nm, pos, gfx1, gfx2) {
    armor = CombatArmor::fromItem(ItemDB::get().getItem(Item::DefaultArmor));
    xpRewardMultiplier = 1;
    lastAttackTime = 0;
}

void CombatEntity::p_notifyAttacked(Entity::Ptr attacker, CombatAttack::Ref atk) {
    CombatEntity* atkr = dynamic_cast<CombatEntity*>(attacker.get());
    if (atkr!=nullptr) {
        double ld = atkr->stats.level - stats.level;
        double lvlMult = (ld>=0) ? (ld*ld+1) : (2.5/abs(ld));
        double atkPower = atk->getPower() * lvlMult;
        double damage = atkPower - armor->getDamageResist();
        stats.health -= damage;
        //TODO - effects and create particle generators from them

        if (DebugOverlays::isOverlayActive(DebugOverlays::CombatData)) {
            cout << endl << left << setw(40) << "Attack: " << atk->getName() << endl;
            cout << left << setw(40) << "Attacker Level: " << atkr->stats.level << left << setw(40) << "\tDefender Level: " << stats.level << endl;
            cout << left << setw(40) << "Weapon Power: " << atk->getPower() << left << setw(40) << "\tArmor Resistance: " << armor->getDamageResist() << endl;
            cout << left << setw(40) << "Power Multiplier: " << lvlMult << left << setw(40) << "\tPower: " << atkPower << endl;
            cout << left << setw(40) << "Damage: " << damage << left << setw(40) << "\tHealth: " << (stats.health+damage) << " -> " << stats.health << endl;
            cout << endl;
        }

        if (stats.health<=0) {
            int levelDiff = stats.level - atkr->stats.level;
            double levelMult = (levelDiff>0) ? (levelDiff*levelDiff) : (0);
            double rewardXp = levelMult*xpRewardMultiplier + stats.level;
            atkr->stats.currentXp += rewardXp; //TODO - level up?

            EntityManager::get().remove(this);
        }
    }
    else
        cout << "Warning: " << getIdString() << " attacked by non CombatEntity " << attacker->getIdString() << endl;

    p_notifyWasAttacked(attacker, atk);
}

void CombatEntity::doAttack(int atkDir) {
    if (Timer::get().timeElapsedSeconds()-lastAttackTime >= weapon->getAttackDelay()) {
        Entity::Ptr atkEnt = AttackEntity::create(EntityManager::get().getEntityPtr(this), weapon, atkDir);
        EntityManager::get().add(atkEnt);
        lastAttackTime = Timer::get().timeElapsedSeconds();
    }
}

EntityStats& CombatEntity::getStats() {
    return stats;
}

CombatArmor::Ref CombatEntity::getArmor() {
    return armor;
}
