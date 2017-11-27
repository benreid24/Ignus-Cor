#ifndef ITEMENTITY_HPP
#define ITEMENTITY_HPP

#include "../Entity.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Data/ItemDB.hpp"

/**
 * Entity class for items
 *
 * \ingroup Entity
 */
class ItemEntity : public Entity {
public:
	/**
	 * Constructs the ItemEntity from the given item id and position
	 */
	ItemEntity(int itemId, sf::Vector2f position) : Entity(ItemDB::getItem(itemId).getName(), position, Properties::ItemMapImagePath+ItemDB::getItem(itemId).getMapImageFile()) { }

    const std::string getType() { return "Item"; }
};

#endif // ITEMENTITY_HPP
