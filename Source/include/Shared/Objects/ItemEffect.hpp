#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <cstdint>
#include <vector>
#include <string>

/**
 * \defgroup Objects
 * \brief Group of classes wrapping objects, meaning Items, Weapons and Spells
 */

/**
 * Helper class for Item effects. Instances can represent lists of base effects
 *
 * \ingroup Objects
 */
class ItemEffect {
public:
    /**
     * Enum for the various effects that can belong to Items
     * Values are powers of 2, so a single effect can actually be a combination of
     * effects by ORing them together
     */
    enum Type: std::uint32_t {
        None = 0,
        DamageHealth = 1,
        RestoreHealth = 2,
        RestoreMagic = 4,
        DamageMagic = 8 //TODO - make effects
    };

private:
    Type type;

public:
    /**
     * Constructs the ItemEffect from the given Type
     */
    ItemEffect(Type type = None);

    /**
     * Implicit construction from int
     */
    ItemEffect(int effect);

    /**
     * Returns whether this effect contains the given base effect
     */
    bool isEffect(Type test) const;

    /**
     * Returns the raw value of the effect
     */
    Type getEffect() const;

    /**
     * Returns the string representation of the effect
     */
    std::string getDescription(const std::string& sep = ", ") const;

    //TODO - function to apply effects to people?

    /**
     * Returns the Type from the index of the type
     */
    static Type getTypeFromIndex(unsigned int index);

    /**
     * Returns the type index for the given base type
     */
    static unsigned int getIndexFromType(Type type);

    /**
     * Returns a list of base effects as strings
     */
    static const std::vector<std::string>& getAllBaseEffects();
};

#endif // EFFECT_HPP
