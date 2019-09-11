#include "Shared/Items/OwnedItem.hpp"
#include "Shared/Data/ItemDB.hpp"
using namespace std;

OwnedItem::OwnedItem(int id)
: OwnedItem(ItemDB::get().getItem(id)) {}

OwnedItem::OwnedItem(Item::ConstPtr i) : item(i) {
    if (item) {
        quantity = 1;
        effects = item->getEffects();
    }
    else
        quantity = 0;
}

Item::ConstPtr OwnedItem::getItem() const {
    return item;
}

const ItemEffect::List& OwnedItem::getAllEffects() const {
    return effects;
}

void OwnedItem::addEffect(const ItemEffect& effect) {
    effects.push_back(effect);
}

void OwnedItem::clearEffects() {
    if (item)
        effects = item->getEffects();
    else
        effects.clear();
}

unsigned int OwnedItem::getQuantity() const {
    return quantity;
}

void OwnedItem::updateQuantity(unsigned int newQ) {
    quantity = newQ;
}
