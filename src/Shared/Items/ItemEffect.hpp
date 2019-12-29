#ifndef ITEMEFFECT_HPP
#define ITEMEFFECT_HPP

#include <list>
#include <vector>
#include <string>

/**
 * \defgroup Items
 * \brief Group of classes wrapping objects, meaning Items, Weapons and Spells
 */

/**
 * Helper class for Item effects
 *
 * \ingroup Items
 */
class ItemEffect {
public:
    /**
     * Enum for the various effects that can belong to Items
     */
    enum Type {
        None,
        DamageHealth,
        RestoreHealth,
        RestoreMagic,
        DamageMagic
    };

private:
    Type type;
    int intensity;
    float chance;
    int duration;

public:
    typedef std::list<ItemEffect> List;

    /**
     * Constructs the ItemEffect from the given Type and intensity
     */
    ItemEffect(Type type = None, int intensity = 0, int dur = 0, float odds = 1);

    /**
     * Implicit construction from int
     */
    ItemEffect(int effect, int intensity = 0, int dur = 0, float odds = 1);

    /**
     * Helper ctor for editor
     */
    ItemEffect(const std::string& name, int intensity, int dur = 0, float odds = 1);

    /**
     * Implicitly cast to the raw enum value of the effect
     */
    operator Type() const { return type; }

    /**
     * Cast to int for convenience
     */
    int asInt() const { return int(type); }

    /**
     * Returns the name of the effect
     */
    const std::string& getName() const;

    /**
     * Returns the string representation of the effect
     */
    const std::string& getDescription() const;

    /**
     * Returns the intensity of the effect
     */
    int getIntensity() const { return intensity; }

    /**
     * Returns the duration of the effect, in ms. 0 means instant
     */
    int getDurationMs() const { return duration; }

    /**
     * Returns the chance of the effect happening
     */
    float getOdds() const;

    //TODO - function to apply effects to people?

    /**
     * Returns a list of base effects as strings
     */
    static const std::vector<std::pair<std::string, std::string> >& getAllEffects();
};

#endif // ITEMEFFECT_HPP
