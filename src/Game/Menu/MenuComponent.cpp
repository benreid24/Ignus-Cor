#include "Game/Menu/MenuComponent.hpp"
using namespace sf;
using namespace std;

MenuComponent::MenuComponent(const string& nm)
: name(nm) {}

const string& MenuComponent::getName() const {
    return name;
}

const Vector2f& MenuComponent::getPosition() const {
    return position;
}

const Vector2f& MenuComponent::getSize() const {
    return size;
}

void MenuComponent::setClickedCallback(Callback cb) {
    clickedCb = cb;
}

void MenuComponent::setMouseoverCallback(Callback cb) {
    mouseOverCb = cb;
}

void MenuComponent::observe(const Event& e) {
    FloatRect bounds(getPosition(), size);
    if (e.type == Event::MouseMoved) {
        if (bounds.contains(e.mouseMove.x, e.mouseMove.y)) {
            if (mouseOverCb)
                mouseOverCb(getName());
        }
    }
    else if (e.type == Event::MouseButtonPressed) {
        if (bounds.contains(e.mouseButton.x, e.mouseButton.y))
            mouseClickedInBounds = true;
        else
            mouseClickedInBounds = false;
    }
    else if (e.type == Event::MouseButtonReleased) {
        if (bounds.contains(e.mouseButton.x, e.mouseButton.y)) {
            if (mouseClickedInBounds && clickedCb)
                clickedCb(getName());
        }
        mouseClickedInBounds = false;
    }
    p_observe(e);
}

void MenuComponent::render(RenderTarget& target, Vector2f pos, Vector2f spos) {
    if (spos.x ==0 && spos.y == 0)
        position = pos;
    else
        position = spos;
    p_render(target, pos);
}
