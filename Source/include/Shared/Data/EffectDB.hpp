#ifndef EFFECTDB_HPP
#define EFFECTDB_HPP

#include <map>
#include "Shared/Combat/CombatEffect.hpp"

/**
 * Static class to store all CombatEffect objects
 *
 * \ingroup Objects
 */
class EffectDB {
    std::map<int, CombatEffect> effects;

    /**
     * Loads all effects from the effect db file
     */
    EffectDB();

public:
    /**
     * Returns a reference to the instance of the effect database
     */
    static EffectDB& get();

	/**
	 * Returns effect information for the given id
	 */
	CombatEffect::Ref getEffect(int id);

	/**
	 * Tells whether or not the given effect exists
	 */
	bool effectExists(int id);

	/**
	 * Removes the effect with the given id
	 */
	void removeEffect(int id);

	/**
	 * Returns a reference to the internal map of effects
	 */
	std::map<int,CombatEffect>& getEffects();

	/**
	 * Saves the effect database to the file
	 */
	void save();
};

#endif // EFFECTDB_HPP
