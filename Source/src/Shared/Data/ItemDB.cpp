#include "Shared/Data/ItemDB.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Properties.hpp"
using namespace std;

ItemDB::ItemDB() {
	File file(Properties::ItemDbFile);
	int count = file.get<uint16_t>();

	for (int i = 0; i<count; ++i) {
        int id = file.get<uint16_t>();
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
        items.emplace(id, Item::Ptr(new Item(id,name,desc,effects,value,mp,mn)));
	}
}

ItemDB& ItemDB::get() {
    static ItemDB db;
    return db;
}

Item::ConstPtr ItemDB::getItem(int id) {
    auto i = items.find(id);
    if (items.end() != i)
        return i->second;
    return nullptr;
}

map<int,Item::Ptr>& ItemDB::getItems() {
    return items;
}

void ItemDB::removeItem(int id) {
	auto i = items.find(id);
	if (items.end() != i)
        items.erase(i);
}

bool ItemDB::itemExists(int id) {
	return items.find(id)!=items.end();
}

void ItemDB::save() {
	File file(Properties::ItemDbFile,File::Out);

	file.write<uint16_t>(items.size());
	for (map<int,Item::Ptr>::iterator i = items.begin(); i!=items.end(); ++i) {
        file.write<uint16_t>(i->second->getId());
        file.writeString(i->second->getName());

        file.write<uint8_t>(i->second->getEffects().size());
        for (auto j = i->second->getEffects().begin(); j != i->second->getEffects().end(); ++j) {
            file.write<uint32_t>(j->asInt());
            file.write<uint32_t>(j->getIntensity());
        }

        file.write<uint32_t>(i->second->getValue());
        file.writeString(i->second->getDescription());
        file.writeString(i->second->getMapImageFile());
        file.writeString(i->second->getMenuImageFile());
	}
}
