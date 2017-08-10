#ifndef ITEMDB_HPP
#define ITEMDB_HPP

#include "Shared/Objects/Item.hpp"
#include <map>

/**
 * Static class to store all data related to items
 *
 * \ingroup Objects
 */
class ItemDB {
	static Item errItem;
	static std::map<int,Item*> items;
	static bool loaded;

public:
	/**
	 * Instances of this class do nothing. Constructor is to load the item db once
	 */
	ItemDB();

	/**
	 * Returns item information for the given id
	 */
	static const Item& getItem(int id);

	/**
	 * Tells whether or not the given item exists
	 */
	static bool itemExists(int id);
};

#endif // ITEMDB_HPP
