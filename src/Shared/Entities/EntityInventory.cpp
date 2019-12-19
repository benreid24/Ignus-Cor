#include "Shared/Entities/EntityInventory.hpp"
#include "Shared/Items/ItemDB.hpp"
#include <algorithm>
using namespace std;

namespace {

struct ItemSearcher {
    const int searchId;
    ItemSearcher(int id) : searchId(id) {}
    bool operator()(const OwnedItem& item) { return item->getId() == searchId; }
};

struct AttackSearcher {
    const int searchId;
    AttackSearcher(int id) : searchId(id) {}
    bool operator()(const CombatAttack::Ref& atk) { return atk->getId() == searchId; }
};

}

EntityInventory::EntityInventory() {
    equippedArmor = CombatArmor::fromItem(ItemDB::get().getItem(Item::DefaultArmor));
    equippedAttack = CombatAttack::fromItem(ItemDB::get().getItem(Item::DefaultWeapon));
}

void EntityInventory::load(File& file) {
    const int numItems = file.get<uint16_t>();
    for (int i = 0; i<numItems; ++i) {
        items.push_back(OwnedItem(file));
    }
    const int numSpells = file.get<uint16_t>();
    for (int i = 0; i<numSpells; ++i) {
        knownSpells.push_back(CombatAttack::fromItem(ItemDB::get().getItem(file.get<uint16_t>())));
    }
    const int armorId = file.get<uint16_t>();
    const int attackId = file.get<uint16_t>();
    equippedArmor = CombatArmor::fromItem(ItemDB::get().getItem(armorId));
    equippedAttack = CombatAttack::fromItem(ItemDB::get().getItem(attackId));
    p_checkEquipped();
}

void EntityInventory::p_checkEquipped() {
    if (!equippedArmor)
        equippedArmor = CombatArmor::fromItem(ItemDB::get().getItem(Item::DefaultArmor));
    else {
        const int armorId = equippedArmor->getId();
        if (amountOwned(armorId) > 0)
            equippedArmor = CombatArmor::fromItem(ItemDB::get().getItem(Item::DefaultArmor));
    }

    if (!equippedAttack)
        equippedAttack = CombatAttack::fromItem(ItemDB::get().getItem(Item::DefaultWeapon));
    else {
        const int attackId = equippedAttack->getId();
        if (amountOwned(attackId) > 0 || knownSpells.end() != find_if(knownSpells.begin(), knownSpells.end(), AttackSearcher(attackId)))
            equippedAttack = CombatAttack::fromItem(ItemDB::get().getItem(Item::DefaultWeapon));
    }
}

unsigned int EntityInventory::amountOwned(int itemId) const {
    auto item = find_if(items.begin(), items.end(), ItemSearcher(itemId));
    if (item != items.end())
        return item->getQuantity();
    return 0;
}

bool EntityInventory::removeItems(int itemId, unsigned int q) {
    auto item = find_if(items.begin(), items.end(), ItemSearcher(itemId));
    if (item != items.end()) {
        if (item->getQuantity() > q)
            item->updateQuantity(item->getQuantity() - q);
        else if (item->getQuantity() == q) {
            items.erase(item);
            p_checkEquipped();
        }
        else
            return false;
        return true;
    }
    return false;
}

void EntityInventory::addItems(int itemId, unsigned int q) {
    Item::ConstPtr data = ItemDB::get().getItem(itemId);
    if (!data) {
        cout << "Error: Added invalid item id (" << itemId << ") to inventory" << endl;
        return;
    }
    auto item = find_if(items.begin(), items.end(), ItemSearcher(itemId));
    if (item == items.end()) {
        item = items.emplace(items.end(), data);
        item->updateQuantity(q);
    }
    else
        item->updateQuantity(item->getQuantity() + q);
}

OwnedItem* EntityInventory::getItem(int itemId) {
    auto item = find_if(items.begin(), items.end(), ItemSearcher(itemId));
    if (item != items.end())
        return &(*item);
    return nullptr;
}

const OwnedItem::List& EntityInventory::getAllItems() const {
    return items;
}

CombatAttack::Ref EntityInventory::getEquippedAttack() const {
    return equippedAttack;
}

CombatArmor::Ref EntityInventory::getEquippedArmor() const {
    return equippedArmor;
}

const vector<CombatAttack::Ref>& EntityInventory::getKnownSpells() const {
    return knownSpells;
}

void EntityInventory::notifyItemUsed(int itemId) {
    auto iter = find_if(items.begin(), items.end(), ItemSearcher(itemId));
    if (iter == items.end()) {
        cout << "Warning: Item " << itemId << " used but not in inventory" << endl;
        return;
    }
    const OwnedItem& item = *iter;

    switch (item->getCategory()) {
        case Item::Armor:
            equippedArmor = CombatArmor::fromItem(item);
            break;
        case Item::Weapon:
            equippedAttack = CombatAttack::fromItem(item);
            break;
        case Item::SpellTomb:
            equippedAttack = CombatAttack::fromItem(item);
            if (knownSpells.end() == find_if(knownSpells.begin(), knownSpells.end(), AttackSearcher(itemId))) {
                knownSpells.push_back(equippedAttack);
                removeItems(itemId, 1);
            }
            break;
        case Item::Food:
        case Item::Potion:
        case Item::Misc: //TODO - maybe on case by case here? or add consumable parameter to item
            removeItems(itemId, 1);
            break;
        case Item::Key:
            break;
        default:
            cout << "Error: Item " << itemId << " has invalid category " << item->getCategory() << endl;
            break;
    }
}
