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
	/**
	 * Constructs the ItemEntity from the given item id and position
	 */
	ItemEntity(int itemId, EntityPosition position) :
	    Entity(ItemDB::getItem(itemId).getName(), position, Properties::ItemMapImagePath+ItemDB::getItem(itemId).getMapImageFile()) { }

public:
	/**
	 * Constructs the ItemEntity from the given item id and position
	 */
	static Entity::Ptr create(int itemId, EntityPosition position) { return Entity::Ptr(new ItemEntity(itemId, position)); }

    const std::string getType() { return "Item"; }
};

#endif // ITEMENTITY_HPP
