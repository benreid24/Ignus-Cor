#include "Shared/Data/ItemDB.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Properties.hpp"
using namespace std;

map<int,Item*> ItemDB::items;
Item ItemDB::errItem(-1,"ERROR","item id not found", Effects::Effect(0), 0, 0, "", "");
bool ItemDB::loaded = false;

namespace {
ItemDB itemDbLoader;
}

ItemDB::ItemDB() {
	if (ItemDB::loaded)
		return;

	File file(Properties::ItemDbFile);
	int count = file.get<uint16_t>();

	for (int i = 0; i<count; ++i) {
        int id = file.get<uint16_t>();
        string name = file.getString();
        Effects::Effect effect = Effects::Effect(file.get<uint32_t>());
        int intensity = file.get<uint32_t>();
        int value = file.get<uint32_t>();
        string desc = file.getString();
        string mp = file.getString();
        string mn = file.getString();
        ItemDB::items[id] = new Item(id,name,desc,effect,intensity,value,mp,mn);
	}
	ItemDB::loaded = true;
}

Item& ItemDB::getItem(int id) {
	if (!itemExists(id))
		return errItem;
	return *items[id];
}

map<int,Item*>& ItemDB::getItems() {
    return items;
}

void ItemDB::removeItem(int id) {
	if (items.find(id)!=items.end()) {
		delete items[id];
		items.erase(id);
	}
}

bool ItemDB::itemExists(int id) {
	return items.find(id)!=items.end();
}

void ItemDB::save() {
	File file(Properties::ItemDbFile,File::Out);

	file.write<uint16_t>(items.size());
	for (map<int,Item*>::iterator i = items.begin(); i!=items.end(); ++i) {
        file.write<uint16_t>(i->second->getId());
        file.writeString(i->second->getName());
        file.write<uint32_t>(i->second->getEffect());
        file.write<uint32_t>(i->second->getIntensity());
        file.write<uint32_t>(i->second->getValue());
        file.writeString(i->second->getDescription());
        file.writeString(i->second->getMapImageFile());
        file.writeString(i->second->getMenuImageFile());
	}
}
