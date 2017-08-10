#ifndef ITEM_HPP
#define ITEM_HPP

#include "Shared/Util/ResourcePool.hpp"
#include "Shared/Objects/Effect.hpp"
#include <string>

/**
 * Class to represent static data regarding items. This is used primarily for the global data store of item information
 * Item instances will just be id's
 */
class Item {
	int id, value, intensity;
	std::string name, description, mapImg, menuImg;
	Effects::Effect effect;

public:
    /**
     * Constructs the item record from the given data
     */
	Item(int id, std::string name, std::string desc, Effects::Effect effect, int intensity, int value, std::string mapImg, std::string menuImg);

	int getId();

	std::string getName();

	std::string getDescription();

    Effects::Effect getEffect();

    int getIntensity();

    int getValue();

    std::string getMapImageFile();

    std::string getMenuImageFile();
};

#endif // ITEM_HPP
