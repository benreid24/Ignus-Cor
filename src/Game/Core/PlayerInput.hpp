#ifndef PLAYERINPUT_HPP
#define PLAYERINPUT_HPP

#include <SFML/Graphics.hpp>
#include "Game/Core/EventObserver.hpp"
#include "Shared/Util/File.hpp"

class BaseState;

/**
 * Helper singleton class to process player input. Listens to window events
 *
 * \ingroup Player
 */
class PlayerInput : public EventObserver {
public:
    /**
     * Helper struct representing an assigned control
     */
    struct Control {
        enum Type {
            Unset,
            KeyboardKey,
            MouseBut
        }type;
        sf::Keyboard::Key key;
        sf::Mouse::Button button;

        Control() : type(Unset) {}
        Control(const sf::Keyboard::Key& k);
        Control(const sf::Mouse::Button& b);
        bool active();
    };
    /**
     * Enum representing the different controls
     */
    enum InputType: unsigned {
        Up = 0,
        Right,
        Left,
        Down,
        Sprint,
        Interact,
        Attack,
        Pause,
        NUM_INPUTS
        //TODO - controls for switching weapons
    };

    /**
     * Returns the PlayerInput object
     */
    static PlayerInput& get();

    /**
     * Loads the input settings from the file
     */
    void load(File& file);

    /**
     * Saves the input settings to the file
     */
    void save(File& file);

    /**
     * Returns whether or not the given input is active
     */
    bool inputActive(InputType input);

    /**
     * Returns whether or not the screen was clicked in the previous frame
     */
    bool screenClicked();

    /**
     * Returns the location of the last screen click, in pixels
     */
    sf::Vector2f clickPosition();

private:
    Control controls[NUM_INPUTS];
    sf::Vector2f clickPos;
    bool clicked;

    PlayerInput(const PlayerInput&) = delete;
    PlayerInput& operator=(const PlayerInput&) = delete;

    /**
     * Sets the default controls
     */
    PlayerInput();

    /**
     * Processes a window event. Listens for clicks
     */
    virtual void observe(const sf::Event& event) override;

    /**
     * Resets the internal state before processing window events
     */
    virtual void notifyFrameStart() override;
};

#endif // PLAYERINPUT_HPP
