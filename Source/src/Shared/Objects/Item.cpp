#include "Shared/Objects/Item.hpp"
using namespace std;

Item::Item(int i, const string& nm, const string& desc, ItemEffect eft, int intense, int v, const string& mp, const string& mn) {
	id = i;
	name = nm;
	description = desc;
	effect = eft;
	intensity = intense;
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

const ItemEffect& Item::getEffect() const {
	return effect;
}

int Item::getIntensity() const {
	return intensity;
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
