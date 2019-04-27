#ifndef COMBATARMOR_HPP
#define COMBATARMOR_HPP

#include <list>
#include <string>

/**
 * Represents effects that armor can have
 *
 * \ingroup Combat
 */
struct CombatArmorEffect {
    enum Type {
        None,
        ResistFire,
        ResistIce,
        ResistElectricity,
        ResistPoison
    };
    Type type;
    double intensity, chance;

    CombatArmorEffect() : type(None) {}

    CombatArmorEffect(Type type, double intensity, double chance) : type(type), intensity(intensity), chance(chance) {}
};

/**
 * Armor that Entities can wear to help negate combat damage
 *
 * \ingroup Combat
 */
class CombatArmor {
    //TODO - graphics change?
    std::string name, description;
    double damageResist;
    std::list<CombatArmorEffect> effects;

public:
    /**
     * Constructs from all parameters
     */
    CombatArmor(const std::string& name, const std::string& description, double damageResistance, const std::list<CombatArmorEffect>& effects);

    /**
     * Returns the name
     */
    std::string getName() const;

    /**
     * Returns the description
     */
    std::string getDescription() const;

    /**
     * Returns the damage resistance
     */
    double getDamageResist() const;

    /**
     * Returns the list of effects
     */
    std::list<CombatArmorEffect> getEffects() const;
};

#endif // COMBATARMOR_HPP
