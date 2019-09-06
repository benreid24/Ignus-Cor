#ifndef ATTACKDB_HPP
#define ATTACKDB_HPP

#include "Shared/Combat/CombatAttack.hpp"
#include <map>

/**
 * Static class to store all data related to CombatAttack's
 *
 * \ingroup Objects
 */
class AttackDB {
	std::map<int,CombatAttack::Ptr> attacks;

	/**
	 * Loads the database from the file
	 */
    AttackDB();

public:
    /**
     * Returns a reference to the instance of the attack database
     */
    static AttackDB& get();

	/**
	 * Returns attack information for the given id
	 */
	CombatAttack::ConstPtr getAttack(int id);

	/**
	 * Tells whether or not the given attack exists
	 */
	bool attackExists(int id);

	/**
	 * Removes the attack with the given id
	 */
	void removeAttack(int id);

	/**
	 * Returns a reference to the internal map of attacks
	 */
	std::map<int,CombatAttack::Ptr>& getAttacks();

	/**
	 * Saves the attack database to the file
	 */
	void save();
};

#endif // ATTACKDB_HPP

