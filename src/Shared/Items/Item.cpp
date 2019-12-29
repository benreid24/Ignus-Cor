#include "Shared/Items/Item.hpp"
using namespace std;

constexpr int Item::DefaultArmor;
constexpr int Item::DefaultSpell;
constexpr int Item::DefaultWeapon;

namespace {
const string invalid = "~~INVALID~~";

const vector<string> categories = {
    "All",
    "Armor",
    "Weapons",
    "Potions",
    "Food",
    "Spell Tombs",
    "Keys",
    "Miscellaneous"
};

const vector<string> categoriesSingle = {
    "All",
    "Armor",
    "Weapon",
    "Potion",
    "Food",
    "Spell Tomb",
    "Key",
    "Miscellaneous"
};
}

const string& Item::getCategoryString(Category cat) {
    if (cat == All)
        return categories[0];
    unsigned int i = cat;
    if (i<categories.size())
        return categories[i+1];
    return invalid;
}

const string& Item::getCategorySingular(Category cat) {
    if (cat == All)
        return categoriesSingle[0];
    unsigned int i = cat;;
    if (i<categoriesSingle.size())
        return categoriesSingle[i+1];
    return invalid;
}

Item::Category Item::getCategoryFromName(const string& name) {
    for (unsigned int i = 0; i<categories.size(); ++i) {
        if (categories[i] == name || categoriesSingle[i] == name)
            return Category(signed(i)-1);
    }
    return All;
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
	mapGfx = mp;
	menuImg = mn;
}

int Item::getId() const {
	return id;
}

Item::Category Item::getCategory() const {
    return category;
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

const string& Item::getMapGfxFile() const {
	return mapGfx;
}

const string& Item::getMenuImageFile() const {
	return menuImg;
}

void Item::save(File& file) const {
    file.write<uint16_t>(getId());
    file.write<uint8_t>(getCategory());
    file.writeString(getName());

    file.write<uint8_t>(getEffects().size());
    for (auto j = getEffects().begin(); j != getEffects().end(); ++j) {
        file.write<uint16_t>(j->asInt());
        file.write<uint32_t>(j->getIntensity());
        file.write<uint32_t>(j->getDurationMs());
        file.write<uint16_t>(j->getOdds() * 1000);
    }

    file.write<uint32_t>(getValue());
    file.writeString(getDescription());
    file.writeString(getMapGfxFile());
    file.writeString(getMenuImageFile());

    p_save(file);
}
