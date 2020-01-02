#ifndef MAPSCRIPT_HPP
#define MAPSCRIPT_HPP

#include "Shared/Scripts/Script.hpp"
#include "Shared/Entities/Entity.hpp"
#include <SFML/System.hpp>

/**
 * Specialized Script interpreter for Map events
 *
 * \ingroup Scripting
 */
class MapScript : public Script {
public:
    /**
     * Create the MapScript
     *
     * \param scr The script itself
     * \param mapName Name of the Map the event is in
     * \param pos The position of the map event
     * \param size The size of the event region
     * \param triggerEntity The Entity that triggered the Script
     * \param runNumber Number of times the event has previously been triggered
     */
    MapScript(
        const Script& scr, std::string mapName, sf::Vector2i pos,
        sf::Vector2i size, Entity::Ptr triggerEntity, int runNumber
    );

    /**
     * vtable
     */
    virtual ~MapScript() = default;
};

#endif // MAPSCRIPT_HPP
