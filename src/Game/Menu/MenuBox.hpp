#ifndef MENUBOX_HPP
#define MENUBOX_HPP

#include "Game/Menu/MenuComponent.hpp"
#include "Shared/Util/ResourcePool.hpp"

/**
 * Organizational class for packing menu components
 * Components can be packed vertically or horizontally
 * and can have a buffer space between them
 *
 * \ingroup GameMenu
 */
class MenuBox : public MenuComponent {
public:
    typedef std::shared_ptr<MenuBox> Ptr;

    enum PackDirection {
        Vertical,
        Horizontal
    };

    /**
     * Construct the box from a background image and pack direction
     */
    static Ptr create(const std::string& image, PackDirection dir);

    /**
     * Construct the box with the given size and pack direction
     */
    static Ptr create(sf::Vector2f size, PackDirection dir);

    /**
     * Set the buffer between components. Default is (0, 0)
     */
    void setPackBuffer(float buffer);

    /**
     * Enable or disable scrolling if the components do not fit
     */
    void setScrollingEnabled(bool allow);

    /**
     * Packs the component into the box
     */
    void pack(MenuComponent::Ptr component);

private:
    std::vector<MenuComponent::Ptr> children;
    PackDirection packDir;
    float packBuffer;
    bool scrollingEnabled;
    float totalSize, scrollPos;

    sf::RenderTexture boxTarget;
    sf::Sprite box;
    TextureReference bgndTxtr;
    sf::Sprite background;
    sf::RectangleShape scrollbar, scrollChannel;

    const float scrollFactor;
    const float scrollBarWidth;
    const float minScrollbarSize;
    const float scrollSizeFactor;
    const float scrollChannelThickness;

    /**
     * Helper ctor to reduce boilerplate
     */
    MenuBox(PackDirection dir);

    /**
     * Construct the box with the given size and pack direction
     */
    MenuBox(sf::Vector2f size, PackDirection dir);

    /**
     * Construct the box from a background image and pack direction
     */
    MenuBox(const std::string& image, PackDirection dir);

    /**
     * Helper function to recompute total height
     */
    void computeSize();

    /**
     * Listens for mouse scrolled events
     */
    virtual void p_observe(const sf::Event& event) override;

    /**
     * Render the box and child components to the given target
     */
    virtual void p_render(sf::RenderTarget& target, sf::Vector2f pos) override;
};

#endif // MENUBOX_HPP
