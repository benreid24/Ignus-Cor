#ifndef ITEM_HPP
#define ITEM_HPP

#include "Shared/Util/ResourcePool.hpp"
#include "Shared/Items/ItemEffect.hpp"
#include <string>

#ifdef EDITOR
class ItemDbEditor;
#endif

class ItemFactory;

/**
 * Class to represent static data regarding items. This is used primarily for the global data store of item information
 * Item instances will just be id's
 *
 * \ingroup Items
 */
class Item {
public:
    typedef std::shared_ptr<Item> Ptr;
    typedef std::shared_ptr<const Item> ConstPtr;

    virtual ~Item() = default;

    /**
     * Classification enum for items. Some categories have their own subclasses
     * as well. In that case this enum is also used to properly cast
     */
    enum Category {
        Armor = 0,
        Weapon,
        Potion,
        Food,
        SpellTomb,
        Key,
        Misc
    };

    /**
     * Category to string
     */
    static const std::string& getCategoryString(Category category);

    /**
     * Category to singular string. ie Weapon vs Weapons
     */
    static const std::string& getCategorySingular(Category category);

    /**
     * All categories
     */
    static const std::vector<std::string>& getAllCategories();

	/**
	 * Returns the item id
	 */
	int getId() const;

	/**
	 * Returns the category of the item
	 */
    Category getCategory() const;

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

    static constexpr int DefaultWeapon = -1;
    static constexpr int DefaultSpell  = -2;
    static constexpr int DefaultArmor  = -3;

protected:
    int id, value;
    Category category;
	std::string name, description, mapImg, menuImg;
	ItemEffect::List effects;

	#ifdef EDITOR
	friend class ItemDbEditor;
	#endif

	friend class ItemFactory;

	/**
     * Constructs the item record from the given data
     */
	Item(int id, Category category, const std::string& name, const std::string& desc,
         const ItemEffect::List& effects, int value, const std::string& mapImg,
         const std::string& menuImg);

    Item(const Item&) = delete;
    Item& operator=(const Item&) = delete;
};

#endif // ITEM_HPP
