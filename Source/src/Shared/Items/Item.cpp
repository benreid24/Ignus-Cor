#include "Shared/Items/Item.hpp"
using namespace std;

constexpr int Item::DefaultArmor;
constexpr int Item::DefaultSpell;
constexpr int Item::DefaultWeapon;

namespace {
const string invalid = "~~INVALID~~";

const vector<string> categories = {
    "Armor",
    "Weapons",
    "Potions",
    "Spell Tombs",
    "Keys",
    "Miscellaneous"
};

const vector<string> categoriesSingle = {
    "Armor",
    "Weapon",
    "Potion",
    "Spell Tomb",
    "Key",
    "Miscellaneous"
};
}

const string& Item::getCategoryString(Category cat) {
    unsigned int i = cat;
    if (i<categories.size())
        return categories[i];
    return invalid;
}

const string& Item::getCategorySingular(Category cat) {
    unsigned int i = cat;
    if (i<categoriesSingle.size())
        return categoriesSingle[i];
    return invalid;
}

const vector<string>& Item::getAllCategories() {
    return categories;
}

Item::Item(int i, Category cat, const string& nm, const string& desc,
           const ItemEffect::List& eft, int v, const string& mp, const string& mn) {
	id = i;
	category = cat;
	name = nm;
	description = desc;
	effects = eft;
	value = v;
	mapImg = mp;
	menuImg = mn;
}

int Item::getId() const {
	return id;
}

const string& Item::getName() const {
	return name;
}

const string& Item::getDescription() const {
    return description;
}

const ItemEffect::List& Item::getEffects() const {
	return effects;
}

int Item::getValue() const {
	return value;
}

const string& Item::getMapImageFile() const {
	return mapImg;
}

const string& Item::getMenuImageFile() const {
	return menuImg;
}

void Item::save(File& file) const {
    file.write<uint16_t>(getId());
    file.writeString(getName());

    file.write<uint8_t>(getEffects().size());
    for (auto j = getEffects().begin(); j != getEffects().end(); ++j) {
        file.write<uint32_t>(j->asInt());
        file.write<uint32_t>(j->getIntensity());
    }

    file.write<uint32_t>(getValue());
    file.writeString(getDescription());
    file.writeString(getMapImageFile());
    file.writeString(getMenuImageFile());

    p_save(file);
}
