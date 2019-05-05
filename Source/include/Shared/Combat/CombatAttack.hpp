#ifndef COMBATATTACK_HPP
#define COMBATATTACK_HPP

#include <string>
#include <list>
#include "Shared/Combat/CombatEffect.hpp"
#include "Shared/Particles/ParticleGeneratorFactory.hpp"

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
    ParticleGeneratorFactory::Preset impactParticleGenerator;
    float particlePersistTime;

    std::string animation;
    ParticleGeneratorFactory::Preset particleGenerator;
    float impactParticlePersistTime;

public:
    /**
     * Creates the "empty" melee attack Punch
     */
    CombatAttack();

    /**
     * Create the CombatAttack as a melee attack
     */
    CombatAttack(const std::string& name, const std::string& description, double power, float AttackDelay, const std::list<CombatEffect>& effects,
                 const std::string& animation, ParticleGeneratorFactory::Preset particles, float partTime = 0);

    /**
     * Creates a ranged attack
     */
    CombatAttack(const std::string& name, const std::string& description, double power, float attackDelay,
                 const std::list<CombatEffect>& effects, const std::string& animation, ParticleGeneratorFactory::Preset particles,
                 float partTime, double range, double speed, const std::string& impactAnimation = "",
                 ParticleGeneratorFactory::Preset impactParts = ParticleGeneratorFactory::None, float impactPartTime = 0);

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
     * Returns the type of ParticleGenerator the attack uses
     */
    ParticleGeneratorFactory::Preset getParticleType() const;

    /**
     * Returns how long the particles should be generated after the attack is finished
     */
    float getParticlePersistanceTime() const;

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
     * Returns the type of particles the explosion uses
     */
    ParticleGeneratorFactory::Preset getImpactParticles() const;

    /**
     * Returns how long particles should be generated after the impact attack is finished
     */
    float getImpactParticlePersistance() const;

    /**
     * Converts to a CombatAttack for the explosion. Ranged only
     */
    CombatAttack toExplosionAttack() const;
};

#endif // COMBATATTACK_HPP
