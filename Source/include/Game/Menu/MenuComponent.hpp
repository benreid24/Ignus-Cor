#ifndef MENUCOMPONENT_HPP
#define MENUCOMPONENT_HPP

#include <memory>
#include <functional>
#include <SFML/Graphics.hpp>
#include "Game/Core/EventObserver.hpp"

class GameMenu;

/**
 * Base component class for game menu objects
 *
 * \ingroup GameMenu
 */
class MenuComponent : public EventObserver {
public:
    typedef std::shared_ptr<MenuComponent> Ptr;
    typedef std::function<void(const std::string&)> Callback;

    /**
     * Creates the component with the given name
     */
    MenuComponent(const std::string& nm);

    /**
     * Sets the clicked callback
     */
    void setClickedCallback(Callback cb);

    /**
     * Sets the callback for when moused over
     */
    void setMouseoverCallback(Callback cb);

    /**
     * Returns the name of the component
     */
    const std::string& getName() const;

    /**
     * Returns the size of the component on screen
     */
    const sf::Vector2f& getSize() const;

    /**
     * Returns the position last rendered at
     */
    const sf::Vector2f& getPosition() const;

    /**
     * Renders the component to the given target at the given position
     * Also updates the internal position for mouse events
     * Callers can override position with the option screenPos parameter
     */
    void render(sf::RenderTarget& target, sf::Vector2f renderPos, sf::Vector2f screenPos = sf::Vector2f(0,0));

protected:
    sf::Vector2f size; //child classes should set this from p_render

    /**
     * For derived classes to render
     */
    virtual void p_render(sf::RenderTarget& target, sf::Vector2f pos) = 0;

private:
    sf::Vector2f position;
    std::string name;
    Callback mouseOverCb, clickedCb;
    bool mouseClickedInBounds;

    MenuComponent(const MenuComponent&) = delete;
    MenuComponent& operator=(const MenuComponent&) = delete;

    /**
     * For determining when moused over or clicked
     */
    virtual void observe(const sf::Event& event) override final;

    /**
     * Derived classes can also be notified of events
     */
    virtual void p_observe(const sf::Event& event) {}
};

#endif // MENUCOMPONENT_HPP
