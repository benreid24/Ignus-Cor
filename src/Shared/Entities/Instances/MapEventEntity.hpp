#ifndef MAPEVENTENTITY_HPP
#define MAPEVENTENTITY_HPP

#include "Shared/Entities/Entity.hpp"
#include "Shared/Maps/MapEvent.hpp"

/**
 * Entity portion of map events that are triggered via interaction
 * 
 * \ingroup Entities
 * \ingroup Maps
 */
class MapEventEntity : public Entity {
    MapEvent::Ptr event;

public:
    static Entity::Ptr create(EntityPosition pos, MapEvent::Ptr evt) {
        return Entity::Ptr(new MapEventEntity(pos, evt));
    }

    virtual ~MapEventEntity() = default;

    virtual const std::string getType() const override {
        return "MapEventEntity";
    }

private:
    MapEventEntity(EntityPosition pos, MapEvent::Ptr evt)
        : Entity("MapEventEntity", pos, ""), event(evt) {
        boundingBox.left = -event->getSize().x * 16;
        boundingBox.top = -event->getSize().y * 16;
        boundingBox.width = event->getSize().x * 48;
        boundingBox.height = event->getSize().y * 48;
        collisionsEnabled = false;
    }

    virtual void p_notifyInteracted(Entity::Ptr entity) override {
        if (event->checkEntityMovement(entity, entity->getBoundingBox()))
            event->trigger(entity);
    }
};

#endif