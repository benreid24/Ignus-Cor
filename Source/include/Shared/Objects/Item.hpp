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
	int id, value, intensity;
	std::string name, description, mapImg, menuImg;
	ItemEffect effect;

	#ifdef EDITOR
	friend class ItemDbEditor;
	#endif

	friend class ItemDB;

	/**
     * Constructs the item record from the given data
     */
	Item(int id, const std::string& name, const std::string& desc, ItemEffect effect, int intensity, int value, const std::string& mapImg, const std::string& menuImg);

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
    const ItemEffect& getEffect() const;

    /**
     * Returns the intensity of the effect
     */
    int getIntensity() const;

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
};

#endif // ITEM_HPP
