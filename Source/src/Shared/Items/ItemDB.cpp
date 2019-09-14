#include "Shared/Items/ItemDB.hpp"
#include "Shared/Items/ItemFactory.hpp"
#include "Shared/Combat/CombatArmor.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Properties.hpp"
using namespace std;

ItemDB::ItemDB() {
	File file(Properties::ItemDbFile);
	items = ItemFactory::getItems(file);
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

	unsigned int sz = 0;
	for (map<int,Item::Ptr>::iterator i = items.begin(); i!=items.end(); ++i) {
        if (i->second->getId() >= 0)
            sz += 1;
	}
	file.write<uint16_t>(sz);
	for (map<int,Item::Ptr>::iterator i = items.begin(); i!=items.end(); ++i) {
        if (i->second->getId() >= 0)
            i->second->save(file);
	}
}
