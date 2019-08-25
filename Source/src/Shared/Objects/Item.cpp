#include "Shared/Objects/Item.hpp"
using namespace std;

Item::Item(int i, std::string nm, std::string desc, Effects::Effect eft, int intense, int v, std::string mp, std::string mn) {
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

string Item::getName() const {
	return name;
}

string Item::getDescription() const {
    return description;
}

Effects::Effect Item::getEffect() const {
	return effect;
}

int Item::getIntensity() const {
	return intensity;
}

int Item::getValue() const {
	return value;
}

string Item::getMapImageFile() const {
	return mapImg;
}

string Item::getMenuImageFile() const {
	return menuImg;
}
