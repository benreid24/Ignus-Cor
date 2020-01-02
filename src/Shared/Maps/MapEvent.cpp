#include "Shared/Maps/MapEvent.hpp"
#include "Shared/Scripts/ScriptManager.hpp"
#include "Shared/Maps/MapManager.hpp"
#include "Shared/Entities/Instances/MapEventEntity.hpp"
#include "Shared/Entities/EntityManager.hpp"

MapEvent::Ptr MapEvent::create(const std::string& mapName, File& file) {
    MapEvent::Ptr event(new MapEvent(mapName, file));
    EntityPosition pos;
    pos.coords.x = event->getPosition().x*32;
    pos.coords.y = event->getPosition().y*32;
    pos.mapName = mapName;
    EntityManager::get().add(MapEventEntity::create(pos, event));
    return event;
}

MapEvent::Ptr MapEvent::create(const MapEvent& event) {
    return MapEvent::Ptr(new MapEvent(event));
}

MapEvent::MapEvent(const std::string& map, File& file) {
    mapName = map;
    type = (EventType)file.get<uint8_t>();
    position.x = file.get<uint32_t>();
    position.y = file.get<uint32_t>();
    size.x = file.get<uint16_t>();
    size.y = file.get<uint16_t>();
    maxRuns = file.get<uint8_t>();
    triggerType = (TriggerType)file.get<uint8_t>();
    if (OnInteract == triggerType)
        triggerDir = (EntityPosition::Direction)file.get<uint8_t>();

    switch (type) {
        case RunScript:
            scriptStr = file.getString();
            script = std::make_shared<Script>(scriptStr);
            break;

        case LoadMap:
            newMapName = file.getString();
            spawnName = file.getString();
            break;

        default:
            std::cout << "Invalid MapEvent type: " << type << std::endl; //TODO - move output into DebugOverlays
            break;
    }
}

void MapEvent::save(File& file) {
    file.write<uint8_t>(type);
    file.write<uint32_t>(position.x);
    file.write<uint32_t>(position.y);
    file.write<uint16_t>(size.x);
    file.write<uint16_t>(size.y);
    file.write<uint8_t>(maxRuns);
    file.write<uint8_t>(triggerType);
    if (OnInteract == triggerType)
        file.write<uint8_t>(triggerDir);

    switch (type) {
        case RunScript:
            file.writeString(scriptStr);
            break;

        case LoadMap:
            file.writeString(newMapName);
            file.writeString(spawnName);
            break;

        default:
            std::cout << "Invalid MapEvent type: " << type << std::endl; //TODO - move output into DebugOverlays
            break;
    }
}

bool MapEvent::checkEntityMovement(Entity::Ptr entity, const sf::FloatRect& oldBox) {
    const sf::FloatRect eventBox(position.x*32, position.y*32, size.x*32, size.y*32);
    const bool inNow = entity->getBoundingBox().intersects(eventBox);
    const bool wasIn = oldBox.intersects(eventBox);
    bool triggered = false;

    switch (triggerType) {
        case StepIn:
            triggered = inNow && !wasIn;
            break;
        case StepOut:
            triggered = !inNow && wasIn;
            break;
        case StepInOrOut:
            triggered = (inNow && !wasIn) || (!inNow && wasIn);
            break;
        case WhileIn:
            triggered = inNow;
            break;
        case OnInteract:
            // cannot know to trigger, but can let caller know
            return inNow && (entity->getPosition().dir==triggerDir || triggerDir==EntityPosition::Any); 
    }

    if (triggered)
        trigger(entity);
    return triggered;
}

void MapEvent::trigger(Entity::Ptr entity) {
    if (curRuns >= maxRuns && maxRuns > 0)
        return;

    switch (type) {
        case RunScript: {
            Script::Ptr scr = std::make_shared<Script>(*script);
            if (entity) {
                scr = std::make_shared<MapScript>(
                    *script,
                    mapName,
                    position,
                    size,
                    entity,
                    curRuns
                );
            }
            ScriptManager::get().runScript(scr);
            break;
        }

        case LoadMap:
            MapManager::get().mapChange(entity, newMapName, spawnName);
            break;

        default:
            std::cout << "Invalid MapEvent type: " << type << std::endl; //TODO - move output into DebugOverlays
            break;
    }

    ++curRuns;
}

// TODO - check when player enters/exits map and trigger onload/unload scripts then
MapEvent::TriggerType MapEvent::getTriggerType() const {
    return triggerType;
}

sf::Vector2i MapEvent::getPosition() const {
    return position;
}

sf::Vector2i MapEvent::getSize() const {
    return size;
}