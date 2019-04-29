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
    double range, speed;

public:
    /**
     * Creates the ranged attack from all the parameters
     */
    CombatRangedAttack(const std::string& name, const std::string& description, double power, const std::list<CombatEffect>& effects, const std::string& animation,
                       double range, double speed, const std::string& impactAnimation = "");

    /**
     * vtable
     */
    virtual ~CombatRangedAttack() = default;

    /**
     * Returns the range of the attack
     */
    double getRange() const;

    /**
     * Returns the speed the attack moves at
     */
    double getSpeed() const;

    /**
     * Returns the file of the impact animation, if any
     */
    std::string getImpactAnimation() const;

    /**
     * Converts to a CombatAttack for the explosion
     */
    CombatAttack toExplosionAttack() const;
};

#endif // COMBATRANGEDATTACK_HPP