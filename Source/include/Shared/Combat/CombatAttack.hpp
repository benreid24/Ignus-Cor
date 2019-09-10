#ifndef COMBATATTACK_HPP
#define COMBATATTACK_HPP

#include <string>
#include <list>
#include <memory>
#include "Shared/Util/File.hpp"
#include "Shared/Combat/CombatEffect.hpp"
#include "Shared/Particles/ParticleGeneratorFactory.hpp"

class Entity;
class AttackDB;

/**
 * \defgroup Combat
 * \brief Classes and code related to Combat between Entity objects
 */

 /**
  * Base class representing all Attacks in the game, both ranged, melee, and spells
  *
  * \ingroup Combat
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
    CombatEffect::List effects;
    float delaySeconds;

    std::string animation;
    ParticleGeneratorFactory::Preset particleGenerator;
    float impactParticlePersistTime;

    //ranged only
    std::string impactAnimation;
    double range, speed;
    ParticleGeneratorFactory::Preset impactParticleGenerator;
    float particlePersistTime;

    friend class AttackDB;

public:
    typedef std::shared_ptr<CombatAttack> Ptr;
    typedef std::shared_ptr<const CombatAttack> ConstPtr;

    /**
     * Creates the "empty" melee attack Punch
     */
    CombatAttack();

    /**
     * Loads the attack from the given file
     */
    CombatAttack(File& file);

    /**
     * Create the CombatAttack as a melee attack
     */
    CombatAttack(const std::string& name, const std::string& description, double power, float AttackDelay, const CombatEffect::List& effects,
                 const std::string& animation, ParticleGeneratorFactory::Preset particles, float partTime = 0);

    /**
     * Creates a ranged attack
     */
    CombatAttack(const std::string& name, const std::string& description, double power, float attackDelay,
                 const CombatEffect::List& effects, const std::string& animation, ParticleGeneratorFactory::Preset particles,
                 float partTime, double range, double speed, const std::string& impactAnimation = "",
                 ParticleGeneratorFactory::Preset impactParts = ParticleGeneratorFactory::None, float impactPartTime = 0);

    /**
     * Saves the attack to the given file
     */
    void save(File& file) const;

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
    const CombatEffect::List& getEffects() const;

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
    CombatAttack::ConstPtr toExplosionAttack() const;
};

#endif // COMBATATTACK_HPP
