#ifndef CREATUREENTITY_HPP
#define CREATUREENTITY_HPP

#include "Shared/Entities/Virtual/AiEntity.hpp"
#include "Shared/Util/File.hpp"

/**
 * Entity class for Creatures, ie Entities that should not persist. Typically created from spawners
 *
 * \ingroup Entity
 */
class CreatureEntity : public AiEntity {
    /**
     * Creates the CreatureEntity from the given data and data file
     */
    CreatureEntity(std::string nm, EntityPosition pos, std::string gfx1, std::string gfx2, File& dataFile);

public:
    /**
     * Loads the Creature from the data file and returns a Ptr to the created object
     */
    static Entity::Ptr create(std::string dataFile); //TODO - create from spawner

    /**
     * vtable
     */
    virtual ~CreatureEntity() = default;

    /**
     * Returns "CreatureEntity"
     */
    const std::string getType() const;
};

#endif // CREATUREENTITY_HPP

