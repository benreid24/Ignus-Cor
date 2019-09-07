#include "Game/Core/PlayerInput.hpp"
using namespace sf;

PlayerInput::Control::Control(const Keyboard::Key& k)
: type(KeyboardKey), key(k) {}

PlayerInput::Control::Control(const Mouse::Button& b)
: type(MouseBut), button(b) {}

bool PlayerInput::Control::active() {
    switch (type) {
    case KeyboardKey:
        return Keyboard::isKeyPressed(key);
    case MouseBut:
        return Mouse::isButtonPressed(button);
    default:
        return false;
    }
}

PlayerInput& PlayerInput::get() {
    static PlayerInput input;
    return input;
}

PlayerInput::PlayerInput() {
    controls[Up] = Keyboard::W;
    controls[Right] = Keyboard::D;
    controls[Down] = Keyboard::S;
    controls[Left] = Keyboard::A;
    controls[Interact] = Keyboard::E;
    controls[Attack] = Mouse::Left;
    controls[Sprint] = Keyboard::LShift;
    controls[Pause] = Keyboard::Return;
}

bool PlayerInput::inputActive(InputType input) {
    if (input < NUM_INPUTS)
        return controls[input].active();
    return false;
}

bool PlayerInput::screenClicked() {
    return clicked;
}

Vector2f PlayerInput::clickPosition() {
    return clickPos;
}

void PlayerInput::reset() {
    clicked = false;
    clickPos = Vector2f(-1,-1);
}

void PlayerInput::process(const RenderWindow& window, const Event& event) {
    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Right) {
            clicked = true;
            clickPos = Vector2f(event.mouseButton.x, event.mouseButton.y);
        }
    }
}

void PlayerInput::load(File& file) {
    //TODO - load player controls
}

void PlayerInput::save(File& file) {
    //TODO - save  player controls
}
