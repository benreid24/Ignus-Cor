#ifndef ITEM_HPP
#define ITEM_HPP

#include "Shared/Util/ResourcePool.hpp"
#include "Shared/Objects/ItemEffect.hpp"
#include <string>

#ifdef EDITOR
class ItemDbEditor;
#endif

class ItemDB;

/**
 * Class to represent static data regarding items. This is used primarily for the global data store of item information
 * Item instances will just be id's
 */
class Item {
public:
    typedef std::shared_ptr<Item> Ptr;
    typedef std::shared_ptr<const Item> ConstPtr;

	/**
	 * Returns the item id
	 */
	int getId() const;

	/**
	 * Returns the item name
	 */
	const std::string& getName() const;

	/**
	 * Returns the item description
	 */
	const std::string& getDescription() const;

	/**
	 * Returns the item's effect
	 */
    const ItemEffect::List& getEffects() const;

    /**
     * Returns the sale value of the item
     */
    int getValue() const;

    /**
     * Returns the image of the item while on the map
     */
    const std::string& getMapImageFile() const;

    /**
     * Returns the image of the item in the menu
     */
    const std::string& getMenuImageFile() const;

public:
    int id, value;
	std::string name, description, mapImg, menuImg;
	ItemEffect::List effects;

	#ifdef EDITOR
	friend class ItemDbEditor;
	#endif

	friend class ItemDB;

	/**
     * Constructs the item record from the given data
     */
	Item(int id, const std::string& name, const std::string& desc, const ItemEffect::List& effects,
      int value, const std::string& mapImg, const std::string& menuImg);
};

#endif // ITEM_HPP
