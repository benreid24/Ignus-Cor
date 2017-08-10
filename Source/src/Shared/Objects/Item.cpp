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

int Item::getId() {
	return id;
}

string Item::getName() {
	return name;
}

string Item::getDescription() {
    return description;
}

Effects::Effect Item::getEffect() {
	return effect;
}

int Item::getIntensity() {
	return intensity;
}

int Item::getValue() {
	return value;
}

string Item::getMapImageFile() {
	return mapImg;
}

string Item::getMenuImageFile() {
	return menuImg;
}
