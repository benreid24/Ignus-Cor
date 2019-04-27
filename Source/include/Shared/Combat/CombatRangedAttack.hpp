#ifndef COMBATRANGEDATTACK_HPP
#define COMBATRANGEDATTACK_HPP

#include "Shared/Combat/CombatAttack.hpp"

/**
 * A ranged attack. Can be physical or spells
 *
 * \ingroup Combat
 */
class CombatRangedAttack : public CombatAttack {
    std::string impactAnimation;
    double range, speed, explosionRadius;

public:
    CombatRangedAttack(const std::string& name, const std::string& description, double power, const std::list<CombatEffect>& effects, const std::string& animation,
                       double range, double speed, const std::string& impactAnimation = "", double explosionRadius = 0);

    /**
     * Returns the range of the attack
     */
    double getRange();

    /**
     * Returns the speed the attack moves at
     */
    double getSpeed();

    /**
     * Returns the explosion radius on impact. 0 means no explosion
     */
    double getExplosionRadius();

    /**
     * Returns the file of the impact animation, if any
     */
    std::string getImpactAnimation();
};

#endif // COMBATRANGEDATTACK_HPP
