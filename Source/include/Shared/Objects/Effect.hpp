#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <cstdint>

/**
 * \defgroup Objects
 * \brief Group of classes wrapping objects, meaning Items, Weapons and Spells
 */

namespace Effects {
/**
 * Enum for the various effects that can belong to Items, Weapons and Spells
 * Values are powers of 2, so a single effect can actually be a combination of
 * effects by ORing them together
 *
 * \ingroup Objects
 */
enum Effect: std::uint32_t {
	DamageHealth = 1,
	RestoreHealth = 2,
    RestoreMagic = 4,
    DamageMagic = 8 //TODO - make effects
};

/**
 * Determines whether the given effect contains the effect to test for
 *
 * \param value The Effect in question
 * \param test The Effect to see if contained
 * \return True if test is contained in value, false otherwise
 */
bool isEffect(Effect value, Effect test);

//TODO - function to apply effects to people?

}

#endif // EFFECT_HPP
