#ifndef ENTITYINVENTORY_HPP
#define ENTITYENVENTORY_HPP

#include <list>
#include <vector>
#include "Shared/Items/OwnedItem.hpp"
#include "Shared/Combat/CombatArmor.hpp"
#include "Shared/Combat/CombatAttack.hpp"
#include "Shared/Util/File.hpp"

/**
 * Stores all Item objects owned by an Entity. Handles armor, weapons, and known spells as well
 *
 * \ingroup Entity
 * \ingroup Items
 */
class EntityInventory
{
    OwnedItem::List items;
    std::vector<CombatAttack::Ref> knownSpells; //"use" spell item -> learn spell. This list is permanent. TODO - modifiable spells via OwnedItem?
    CombatAttack::Ref equippedAttack;
    CombatArmor::Ref equippedArmor;

    /**
     * Helper function to check validity of equipped armor and weapon
     */
    void p_checkEquipped();

public:
    /**
     * Creates an empty inventory and assigns default weapon and armor
     */
    EntityInventory();

    /**
     * Loads the inventory from the given file
     */
    void load(File& file);

    /**
     * Returns the quantity owned of the given item
     */
    unsigned int amountOwned(int itemId) const;

    /**
     * Removes the given quantity of the given item
     */
    bool removeItems(int itemId, unsigned int quantity);

    /**
     * Adds the given items to the inventory
     */
    void addItems(int itemId, unsigned int quantity = 1);

    /**
     * Notifies the inventory that the given item was used
     */
    void notifyItemUsed(int itemId);

    /**
     * Returns a direct pointer to the OwnedItem, if any
     */
    OwnedItem* getItem(int itemId);

    /**
     * Returns the list of all items
     */
    const OwnedItem::List& getAllItems() const;

    /**
     * Returns the equipped attack
     */
    CombatAttack::Ref getEquippedAttack() const;

    /**
     * Returns the equipped armor
     */
    CombatArmor::Ref getEquippedArmor() const;

    /**
     * Returns the list of spells known
     */
    const std::vector<CombatAttack::Ref>& getKnownSpells() const;
};

#endif // ENTITYINVENTORY_HPP
