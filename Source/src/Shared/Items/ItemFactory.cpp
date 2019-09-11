#include "Shared/Items/ItemFactory.hpp"
#include "Shared/Combat/CombatArmor.hpp"
using namespace std;

map<int, Item::Ptr> ItemFactory::getItems(File& file) {
    map<int, Item::Ptr> items;

    int count = file.get<uint16_t>();
	for (int i = 0; i<count; ++i) {
        int id = file.get<uint16_t>();
        Item::Category cat = (Item::Category)file.get<uint8_t>();
        string name = file.getString();

        ItemEffect::List effects;
        int nEffects = file.get<uint8_t>();
        for (int j = 0; j<nEffects; ++j) {
            ItemEffect::Type t = ItemEffect::Type(file.get<uint32_t>());
            int intensity = file.get<uint32_t>();
            effects.emplace_back(t, intensity);
        }

        int value = file.get<uint32_t>();
        string desc = file.getString();
        string mp = file.getString();
        string mn = file.getString();

        if (cat == Item::Armor) {
            double dr = double(file.get<uint32_t>()) / 100.0;
            items.emplace(id, Item::Ptr(new CombatArmor(id,name,desc,effects,value,mp,mn,dr)));
        }
        else if (cat == Item::Weapon) {
            //TODO - load weapons as items
        }
        else {
            items.emplace(id, Item::Ptr(new Item(id,cat,name,desc,effects,value,mp,mn)));
        }
	}

    return items;
}
