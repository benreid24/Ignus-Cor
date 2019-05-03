#ifndef COMBATATTACK_HPP
#define COMBATATTACK_HPP

#include <string>
#include <list>
#include "Shared/Combat/CombatEffect.hpp"

class Entity;

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
public:
    enum Type {
        Melee,
        Ranged
    };

private:
    Type type;
    std::string name, description;
    double power;
    std::list<CombatEffect> effects;
    float delaySeconds;

    //ranged only
    std::string impactAnimation;
    double range, speed;

    std::string animation;
    //TODO - link to particle generator?

public:
    /**
     * Creates the "empty" melee attack Punch
     */
    CombatAttack();

    /**
     * Create the CombatAttack as a melee attack
     */
    CombatAttack(const std::string& name, const std::string& description, double power, float AttackDelay,
                 const std::list<CombatEffect>& effects, const std::string& animation);

    /**
     * Creates a ranged attack
     */
    CombatAttack(const std::string& name, const std::string& description, double power, float attackDelay,
                 const std::list<CombatEffect>& effects, const std::string& animation, double range,
                 double speed, const std::string& impactAnimation = "");

    /**
     * Returns the type of the attack, either Melee or Ranged
     */
    Type getType() const;

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
     * Returns the delay between attacks, in seconds
     */
    float getAttackDelay() const;

    /**
     * Returns the effects of the CombatAttack
     */
    std::list<CombatEffect> getEffects() const;

    /**
     * Returns the animation file for the attack
     */
    std::string getAnimation() const;

    /**
     * Returns the range of the attack. Ranged only
     */
    double getRange() const;

    /**
     * Returns the speed the attack moves at. Ranged only
     */
    double getSpeed() const;

    /**
     * Returns the file of the impact animation, if any. Ranged only
     */
    std::string getImpactAnimation() const;

    /**
     * Converts to a CombatAttack for the explosion. Ranged only
     */
    CombatAttack toExplosionAttack() const;
};

#endif // COMBATATTACK_HPP
