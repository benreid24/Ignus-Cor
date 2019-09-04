#ifndef NPCENTITY_HPP
#define NPCENTITY_HPP

#include "Shared/Entities/Virtual/AiEntity.hpp"
#include "Shared/Util/File.hpp"

/**
 * Entity class for NPC's. Can be creatures too, but NPC's are special in that their state persists
 * CreatureEntity is for Entities that are generated by spawners or otherwise should not persist
 *
 * \ingroup Entity
 */
class NpcEntity : public AiEntity {
    /**
     * Creates the NpcEntity from the given data and data file
     */
    NpcEntity(std::string nm, EntityPosition pos, std::string gfx1, std::string gfx2);

public:
    /**
     * Loads the NPC from the data file and returns a Ptr to the created object
     */
    static Entity::Ptr create(std::string dataFile);

    /**
     * Creates an NpcEntity from the given data
     */
    static Entity::Ptr create(std::string nm, EntityPosition pos, std::string gfx1, std::string gfx2);

    /**
     * vtable
     */
    virtual ~NpcEntity() = default;

    /**
     * Returns "NpcEntity"
     */
    const std::string getType() const;
};

#endif // NPCENTITY_HPP
