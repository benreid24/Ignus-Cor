#ifndef ITEMENTITY_HPP
#define ITEMENTITY_HPP

#include "Shared/Entities/Entity.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Items/ItemDB.hpp"

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
	    Entity(item->getName(), position, item->getMapGfxFile()) { }

    /**
     * Checks if the interacting Entity is a Player. If yes, gives itself to player then
     * removes itself from the world
     */
    virtual void p_notifyInteracted(Entity::Ptr user) override {
        Entity::notifyInteracted(user);
        if (user->getType()=="Player") {
            //TODO - add to inventory
            EntityManager::get()->remove(this);
        }
    }

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

    /**
     * Returns "Item"
     */
    virtual const std::string getType() const override { return "Item"; }
};

#endif // ITEMENTITY_HPP
