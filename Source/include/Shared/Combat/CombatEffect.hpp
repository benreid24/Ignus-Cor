#ifndef COMBATEFFECT_HPP
#define COMBATEFFECT_HPP

#include <string>

class EffectDB;

/**
 * Describes effects that spells and weapons may apply in addition to damage
 *
 * \ingroup Combat
 */
struct CombatEffect {
    enum Type {
        None,
        Fire,
        Freeze,
        Electrocute,
        Poison
    };

    Type type;
    std::string name, description;
    //TODO - link to particle generator?

    struct Ref {
        const CombatEffect* operator->() const { return effect; }
        Ref(const CombatEffect* ef, double ints, double chnc) : effect(ef), intensity(ints), chance(chnc) {}

        double intensity, chance; //TODO - add duration
        const CombatEffect* effect;
    };

    Ref makeRef() const { return Ref(this, 0, 0); }

private:
    CombatEffect() : type(None) {}
    CombatEffect(Type type, const std::string name, const std::string description)
        : type(type), name(name), description(description) {}

    friend class EffectDB;
};

#endif // COMBATEFFECT_HPP
