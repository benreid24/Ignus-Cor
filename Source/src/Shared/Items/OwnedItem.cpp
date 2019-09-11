#include "Shared/Items/OwnedItem.hpp"
#include "Shared/Items/ItemDB.hpp"
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

OwnedItem::OwnedItem(File& file) {
    item = ItemDB::get().getItem(file.get<uint16_t>());
    quantity = file.get<uint16_t>();
    int neffects = file.get<uint8_t>();
    for (int i = 0; i<neffects; ++i) {
        int effect = file.get<uint16_t>();
        int intensity = file.get<uint32_t>();
        int dur = file.get<uint32_t>();
        float odds = float(file.get<uint16_t>()) / 1000.0;
        effects.push_back(ItemEffect(effect, intensity, dur, odds));
    }
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
