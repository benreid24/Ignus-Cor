#ifndef ITEMDB_HPP
#define ITEMDB_HPP

#include "Shared/Items/Item.hpp"
#include <map>

/**
 * Static class to store all data related to items
 *
 * \ingroup Objects
 */
class ItemDB {
	std::map<int,Item::Ptr> items;

	/**
	 * Loads the items
	 */
	ItemDB();

public:
    /**
     * Returns a reference to the ItemDB
     */
    static ItemDB& get();

	/**
	 * Returns item information for the given id
	 */
	Item::ConstPtr getItem(int id);

	/**
	 * Tells whether or not the given item exists
	 */
	bool itemExists(int id);

	/**
	 * Removes the item with the given id
	 */
	void removeItem(int id);

	/**
	 * Returns a reference to the internal map of items
	 */
	std::map<int,Item::Ptr>& getItems();

	/**
	 * Saves the item database to the file
	 */
	void save();
};

#endif // ITEMDB_HPP
