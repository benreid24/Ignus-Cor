#include "Shared/Items/Item.hpp"
using namespace std;

Item::Item(int i, const string& nm, const string& desc, const ItemEffect::List& eft, int v, const string& mp, const string& mn) {
	id = i;
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
