#ifndef MAPEVENT_HPP
#define MAPEVENT_HPP

#include <string>

#include "Shared/Entities/Entity.hpp"
#include "Shared/Scripts/Specialized/MapScript.hpp"
#include "Shared/Util/File.hpp"

class Map;
class MapEditor;

/**
 * Class to represent an event in a Map
 *
 * \ingroup Maps
 */
class MapEvent {
public:
    enum EventType {
        RunScript,
        LoadMap
    };

    enum TriggerType {
        OnMapLoad,
        StepIn,
        StepOut,
        StepInOrOut,
        WhileIn,
        OnInteract
    };

    typedef std::shared_ptr<MapEvent> Ptr;
    typedef std::weak_ptr<MapEvent> WeakPtr;

    static Ptr create(const std::string& mapName, File& file);
    static Ptr create(const MapEvent& event);

    /**
     * Saves the MapEvent to the given file
     */
    void save(File& file);

    /**
     * Notifies the MapEvent of Entity movement. Returns true if triggered
     */
    bool checkEntityMovement(Entity::Ptr entity, const sf::FloatRect& oldBounds);

    /**
     * Triggers the event if it is permitted to do so
     */
    void trigger(Entity::Ptr entity);

    /**
     * Returns the trigger type
     */
    TriggerType getTriggerType() const;

    /**
     * Returns the position
     */
    sf::Vector2i getPosition() const;

    /**
     * Returns the size
     */
    sf::Vector2i getSize() const;

private:
    /**
     * Loads the MapEvent from the file
     */
    MapEvent(const std::string& mapName, File& file);

    MapEvent() = default;
    MapEvent(const MapEvent&) = default;

    // Common
    EventType type;
    std::string mapName;
    sf::Vector2i position;
    sf::Vector2i size;

    // Trigger
    TriggerType triggerType;
    int maxRuns; 
    int curRuns;
    EntityPosition::Direction triggerDir;

    // Script event
    Script::Ptr script;
    std::string scriptStr;

    // Map load event
    std::string newMapName;
    std::string spawnName;

    friend class Map;
    friend class MapEditor;
};

#endif