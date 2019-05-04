#ifndef PARTICLEGRAPHICS_HPP
#define PARTICLEGRAPHICS_HPP

#include "Shared/Util/ResourcePool.hpp"

/**
 * Helper class to render graphics for particles. Can be an image or Animation. Maybe
 * add support for generated graphics in the future?
 *
 * \ingroup Particles
 */
class ParticleGraphics {
public:
    enum GfxType {
        NoGfx,
        Image,
        Anim
    }gfxType = NoGfx;

    TextureReference texture;
    sf::Sprite sprite;

    AnimationReference animRef;
    Animation animation;

public:
    /**
     * Creates an empty ParticleGraphics
     */
    ParticleGraphics() = default;

    /**
     * Loads from the file
     */
    ParticleGraphics(const std::string& file);

    /**
     * Destroys the graphics
     */
    ~ParticleGraphics() = default;

    /**
     * Loads the image or animation from the given file
     */
    void load(const std::string& file);

    /**
     * Renders to the given target at the given position. Optionally specify rotation
     * and opacity for images only
     */
    void render(sf::RenderTarget& target, sf::Vector2f pos, int rotation = 0, int opacity = 255);
};

#endif // PARTICLEGRAPHICS_HPP
