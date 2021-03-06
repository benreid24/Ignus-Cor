#ifndef GAMEBUTTON_HPP
#define GAMEBUTTON_HPP

#include "Game/Menu/MenuComponent.hpp"
#include "Shared/Util/ResourcePool.hpp"

/**
 * Represents a clickable button on the screen
 *
 * \ingroup GameMenu
 */
class MenuButton : public MenuComponent {
public:
    typedef std::shared_ptr<MenuButton> Ptr;

    /**
     * Specifies the image to use for the button
     */
    enum Style {
        NoImage,  /// Just the text label and mouse over highlight
        Primary,  /// Uses the main button image
        Secondary /// Uses the secondary button image
    };

    /**
     * Creates a button with the given text, style, and size
     */
    static Ptr create(const std::string& text, Style style, sf::Vector2f size = sf::Vector2f(0,0));

    /**
     * Sets the text properties
     */
    void setTextProperties(sf::Color color, float fontSize, bool bold = false, bool italic = false);

private:
    TextureReference texture;
    sf::RenderTexture highlightTxtr;
    sf::Sprite image, highlight;
    sf::Text text;
    sf::Vector2f textOffset;

    /**
     * Constructs from text, style, and size
     */
    MenuButton(const std::string& text, Style style, sf::Vector2f size);

    /**
     * Renders the button to the given target
     */
    virtual void p_render(sf::RenderTarget& target, sf::Vector2f pos) override;
};

#endif // GAMEBUTTON_HPP
