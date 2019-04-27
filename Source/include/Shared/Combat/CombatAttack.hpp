#ifndef COMBATATTACK_HPP
#define COMBATATTACK_HPP

#include <string>
#include <list>
#include "Shared/Combat/CombatEffect.hpp"

/**
 * \defgroup Combat
 * \brief Classes and code related to Combat between Entity objects
 */

 /**
  * Base class representing all Attacks in the game, both ranged, melee, and spells
  *
  * \ingroup CombatAttack
  */
class CombatAttack {
protected:
    std::string name, description;
    double power;
    std::list<CombatEffect> effects;

    std::string animation;
    //TODO - link to particle generator?

public:
    /**
     * Create the CombatAttack from it's members values
     */
    CombatAttack(const std::string& name, const std::string& description, double power, const std::list<CombatEffect>& effects, const std::string& animation);

    /**
     * vtable
     */
    virtual ~CombatAttack() = default;

    /**
     * Returns the name of the CombatAttack
     */
    std::string getName() const;

    /**
     * Returns the description of the CombatAttack
     */
    std::string getDescription() const;

    /**
     * Returns the power of the CombatAttack
     */
    double getPower() const;

    /**
     * Returns the effects of the CombatAttack
     */
    std::list<CombatEffect> getEffects() const;

    /**
     * Returns the animation file for the attack
     */
    std::string getAnimation() const;
};

#endif // COMBATATTACK_HPP
