#ifndef UUID_HPP
#define UUID_HPP

/**
 * Helper class to create unique ids for each run of the game
 */
class UUID {
	static int nextUuid;

public:
	/**
	 * Returns a unique id
	 */
	static int create() { return nextUuid++; }
};

#endif // UUID_HPP
