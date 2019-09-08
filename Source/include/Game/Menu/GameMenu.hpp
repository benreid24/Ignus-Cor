#ifndef GAMEMENU_HPP
#define GAMEMENU_HPP

#include "Game/Menu/MenuComponent.hpp"
#include "Game/Menu/MenuBox.hpp"

/**
 * \defgroup GameMenu Classes related to menus in the game itself
 */

/**
 * Class for managing game menus
 *
 * \ingroup GameMenu
 */
class GameMenu {
public:
    /**
     * Helper enum for the different fixed menu types
     */
    enum Type {
        PauseMenu
    };

    /**
     * Construct an empty menu with the given type
     */
    GameMenu(Type type);

    /**
     * Constructs a menu with no background using the given size and pack direction
     */
    GameMenu(sf::Vector2f size, MenuBox::PackDirection packDir);

    /**
     * Constructs the menu with the given background and pack direction
     */
    GameMenu(const std::string& bgnd, MenuBox::PackDirection packDir);

    /**
     * Sets the position of the menu. Not required if a Type was specified
     */
    void setPosition(sf::Vector2f pos);

    /**
     * Adds the component to the menu
     */
    void addComponent(MenuComponent::Ptr component);

    /**
     * Renders the menu to the given target
     */
    void render(sf::RenderTarget& target);

private:
    MenuBox::Ptr box;
    sf::Vector2f position;
    TextureReference txtr;
    sf::Sprite background;
};

#endif // GAMEMENU_HPP
