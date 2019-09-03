#ifndef ITEMENTITY_HPP
#define ITEMENTITY_HPP

#include "Shared/Entities/Entity.hpp"
#include "Shared/Entities/EntityManager.hpp"
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
	ItemEntity(Item::ConstPtr item, EntityPosition position) :
	    Entity(item->getName(), position, item->getMapImageFile()) { }

public:
    /**
     * Destructor
     */
    virtual ~ItemEntity() = default;

	/**
	 * Constructs the ItemEntity from the given item id and position
	 */
	static Entity::Ptr create(int itemId, EntityPosition position) {
	    Item::ConstPtr item = ItemDB::get().getItem(itemId);
	    if (!item) {
            std::cout << "Error: ItemEntity::create called with invalid item id " << itemId << "\n";
            return nullptr;
	    }
	    return Entity::Ptr(new ItemEntity(item, position));
    }

    const std::string getType() const { return "Item"; }

    void notifyInteracted(Entity::Ptr user) {
        Entity::notifyInteracted(user);
        if (user->getType()=="Player") {
            //TODO - add to inventory
            EntityManager::get()->remove(this);
        }
    }
};

#endif // ITEMENTITY_HPP
