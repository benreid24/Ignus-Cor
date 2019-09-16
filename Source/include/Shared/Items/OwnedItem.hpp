#ifndef OWNEDITEM_HPP
#define OWNEDITEM_HPP

#include "Shared/Items/Item.hpp"

/**
 * Class to represent an owned item in inventory
 *
 * \ingroup Items
 */
class OwnedItem {
    OwnedItem() = delete;

public:
    /**
     * Create by loading from the given file
     */
    OwnedItem(File& file);

    /**
     * Creates from an item id
     */
    OwnedItem(int id);

    /**
     * Create from an item
     */
    OwnedItem(Item::ConstPtr item);

    /**
     * Implicit conversion to Item
     */
    operator Item::ConstPtr() const { return item; }

    /**
     * Explicit conversion
     */
    Item::ConstPtr getItem() const;

    /**
     * Returns the quantity owned. Default is 1
     */
    unsigned int getQuantity() const;

    /**
     * Set the quantity
     */
    void updateQuantity(unsigned int newQ);

    /**
     * Returns all item effects, including those added later
     */
    const ItemEffect::List& getAllEffects() const;

    /**
     * Adds a new effect
     */
    void addEffect(const ItemEffect& effect);

    /**
     * Clears all added effects
     */
    void clearEffects();

    /**
     * Saves to the given file
     */
    void save(File& file) const;

private:
    Item::ConstPtr item;
    unsigned int quantity;
    ItemEffect::List effects;
};

#endif // OWNEDITEM_HPP
