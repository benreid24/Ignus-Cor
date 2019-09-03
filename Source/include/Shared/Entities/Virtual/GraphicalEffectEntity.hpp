#ifndef GRAPHICALEFFECTENTITY_HPP
#define GRAPHICALEFFECTENTITY_HPP

#include "Shared/Entities/Entity.hpp"


/**
 * Virtual base class for graphical effects. Can optionally be bound to another Entity
 */
class GraphicalEffectEntity : public Entity {
    bool removeWithBound;
    sf::Vector2f offset;

protected:
    Entity::WeakPtr boundTo;
    bool wasBound;

    /**
     * Returns true if the entity can be destroyed. Default is always true, but subclasses
     * can override to prevent destruction. Example: Wait for particle generator to finish
     */
    virtual bool canDestroy();

    /**
     * Update logic to be applied before the time variable is updated
     */
    virtual void beforeTimerUpdate() override = 0;

    /**
     * Update logic to be applied after the timer variable is updated
     */
    void afterTimerUpdate() override final;

public:
    /**
     * Creates the graphical Entity and optionally binds to the given Entity
     */
    GraphicalEffectEntity(Entity::Ptr bindTo, EntityPosition pos, std::string gfx1, bool removeWithBound = true);

    /**
     * vtable
     */
    virtual ~GraphicalEffectEntity() = default;

    /**
     * Returns "GraphicalEffectEntity"
     */
    virtual const std::string getType() const = 0;

    /**
     * Just calls Entity::render. Virtual to allow subclasses to override
     */
    virtual void render(sf::RenderTarget& target, sf::Vector2f camPos);
};

#endif // GRAPHICALEFFECTENTITY_HPP
