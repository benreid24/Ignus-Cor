#ifndef COMBATEFFECT_HPP
#define COMBATEFFECT_HPP

#include <string>

/**
 * Describes effects that spells and weapons may apply in addition to damage
 */
struct CombatEffect {
    enum Type {
        None,
        Fire,
        Freeze,
        Electrocute,
        Poison
    };

    CombatEffect() : type(None), intensity(0), chance(0) {}
    CombatEffect(Type type, const std::string name, const std::string description, double intensity, double chance)
        : type(type), name(name), description(description), intensity(intensity), chance(chance) {}

    Type type;
    std::string name, description;
    double intensity, chance;
};

#endif // COMBATEFFECT_HPP
