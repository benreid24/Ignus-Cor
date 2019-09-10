#ifndef COMBATEFFECT_HPP
#define COMBATEFFECT_HPP

#include <string>
#include <list>

/**
 * Describes effects that spells and weapons may apply in addition to damage
 *
 * \ingroup Combat
 */
class CombatEffect {
public:
    typedef std::list<CombatEffect> List;

    ///Actual type of effect
    enum Type {
        None,
        Fire,
        Freeze,
        Electrocute,
        Poison
    };

    /**
     * Construct from type and parameters
     */
    CombatEffect(Type type, double intense, double odds, double duration);

    /**
     * Implicit cast
     */
    CombatEffect(int type, double intense, double odds, double duration);

    /**
     * Implicit conversion
     */
    operator Type() const { return type; }

    /**
     * Explicit conversion
     */
    Type getType() const;

    /**
     * Returns the name of the effect
     */
    const std::string& getName() const;

    /**
     * Returns the description of the effect
     */
    const std::string& getDescription() const;

    /**
     * Returns the intensity of the effect
     */
    double getIntensity() const;

    /**
     * Returns the chance of the effect happening
     */
    double getChance() const;

    /**
     * Returns the duration of the effect, in seconds
     */
    double getDuration() const;

private:
    Type type;
    double intensity, chance, duration;
    //TODO - link to particle generator?
};

#endif // COMBATEFFECT_HPP
