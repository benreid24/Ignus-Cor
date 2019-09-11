#ifndef COMBATATTACK_HPP
#define COMBATATTACK_HPP

#include <string>
#include <list>
#include <memory>
#include "Shared/Items/Item.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Particles/ParticleGeneratorFactory.hpp"

class Entity;
class ItemFactory;

/**
 * \defgroup Combat
 * \brief Classes and code related to Combat between Entity objects
 */

 /**
  * Class representing all Attacks in the game, both ranged, melee, and spells
  *
  * \ingroup Combat
  * \ingroup Items
  */
class CombatAttack : public Item {
public:
    enum Type {
        Melee,
        Ranged
    };

private:
    /**
     * Create the CombatAttack as a melee attack
     */
    CombatAttack(int id, const std::string& name, const std::string& desc,
         const ItemEffect::List& effects, int value, const std::string& mapImg,
         const std::string& menuImg, double power, float attackDelay, const std::string& animation,
         ParticleGeneratorFactory::Preset particles, float partTime = 0);

    /**
     * Creates a ranged attack
     */
    CombatAttack(int id, const std::string& name, const std::string& desc,
         const ItemEffect::List& effects, int value, const std::string& mapImg,
         const std::string& menuImg, double power, float attackDelay,
         const std::string& animation, ParticleGeneratorFactory::Preset particles,
         float partTime, Item::Category cat, double range, double speed,
         const std::string& impactAnimation = "",
         ParticleGeneratorFactory::Preset impactParts = ParticleGeneratorFactory::None,
         float impactPartTime = 0);

    /**
     * Saves a metric fuck ton of data to the file
     */
    virtual void p_save(File& file) const override;

public:
    typedef std::shared_ptr<const CombatAttack> Ref;

    virtual ~CombatAttack() = default;

    /**
     * Helper function to cast to correct pointer type
     */
    static Ref fromItem(Item::ConstPtr item);

    /**
     * Saves the attack to the given file
     */
    void save(File& file) const; //TODO - make virtual

    /**
     * Returns the type of the attack, either Melee or Ranged
     */
    Type getType() const;

    /**
     * Returns the power of the CombatAttack
     */
    double getPower() const;

    /**
     * Returns the delay between attacks, in seconds
     */
    float getAttackDelay() const;

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
    CombatAttack::Ref toExplosionAttack() const;

private:
    Type type;
    std::string name, description;
    double power;
    float delaySeconds;

    std::string animation;
    ParticleGeneratorFactory::Preset particleGenerator;
    float impactParticlePersistTime;

    //ranged only
    std::string impactAnimation;
    double range, speed;
    ParticleGeneratorFactory::Preset impactParticleGenerator;
    float particlePersistTime;

    friend class ItemFactory;
};

#endif // COMBATATTACK_HPP
