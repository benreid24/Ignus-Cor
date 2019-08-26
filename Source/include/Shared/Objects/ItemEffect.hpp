#ifndef ITEMEFFECT_HPP
#define ITEMEFFECT_HPP

#include <list>
#include <vector>
#include <string>

/**
 * \defgroup Objects
 * \brief Group of classes wrapping objects, meaning Items, Weapons and Spells
 */

/**
 * Helper class for Item effects
 *
 * \ingroup Objects
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

public:
    typedef std::list<ItemEffect> List;

    /**
     * Constructs the ItemEffect from the given Type and intensity
     */
    ItemEffect(Type type = None, int intensity = 0);

    /**
     * Implicit construction from int
     */
    ItemEffect(int effect);

    /**
     * Helper ctor for editor
     */
    ItemEffect(const std::string& desc, int intensity);

    /**
     * Implicitly cast to the raw enum value of the effect
     */
    operator Type() const { return type; }

    /**
     * Cast to int for convenience
     */
    int asInt() const { return int(type); }

    /**
     * Returns the string representation of the effect
     */
    const std::string& getDescription() const;

    /**
     * Returns the intensity of the effect
     */
    int getIntensity() const { return intensity; }

    //TODO - function to apply effects to people?

    /**
     * Returns a list of base effects as strings
     */
    static const std::vector<std::string>& getAllEffects();
};

#endif // ITEMEFFECT_HPP
