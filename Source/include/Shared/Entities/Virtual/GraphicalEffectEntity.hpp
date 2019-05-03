#ifndef GRAPHICALEFFECTENTITY_HPP
#define GRAPHICALEFFECTENTITY_HPP

#include "Shared/Entities/Entity.hpp"


/**
 * Virtual base class for graphical effects. Can optionally be bound to another Entity
 */
class GraphicalEffectEntity : public Entity {
    Entity::WeakPtr boundTo;
    bool wasBound;
    sf::Vector2f offset;

public:
    /**
     * Creates the graphical Entity and optionally binds to the given Entity
     */
    GraphicalEffectEntity(Entity::Ptr bindTo, EntityPosition pos, std::string gfx1);

    /**
     * vtable
     */
    virtual ~GraphicalEffectEntity() = default;

    /**
     * If bound, updates the position based on the bound Entity. Deletes if bound Entity no longer exists
     */
    virtual void update();

    /**
     * Just calls Entity::render. Virtual to allow subclasses to override
     */
    virtual void render(sf::RenderTarget& target, sf::Vector2f camPos);
};

#endif // GRAPHICALEFFECTENTITY_HPP
