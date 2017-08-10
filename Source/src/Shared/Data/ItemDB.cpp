#include "Shared/Data/ItemDB.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Properties.hpp"
using namespace std;

namespace {
ItemDB itemDbLoader;
}

Item ItemDB::errItem(-1,"ERROR","item id not found", Effects::Effect(0), 0, 0, "", "");
bool ItemDB::loaded = false;
map<int,Item*> ItemDB::items;

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

const Item& ItemDB::getItem(int id) {
	if (!itemExists(id))
		return errItem;
	return *items[id];
}

bool ItemDB::itemExists(int id) {
	return items.find(id)!=items.end();
}
