#include "Game/Menu/MenuButton.hpp"
#include "Game/Core/Game.hpp"
#include "Shared/Properties.hpp"
using namespace sf;
using namespace std;

MenuButton::Ptr MenuButton::create(const string& text, Style style, Vector2f size) {
    return Ptr(new MenuButton(text, style, size));
}

MenuButton::MenuButton(const string& txt, Style style, Vector2f sz)
: MenuComponent(txt) {
    switch (style) {
    case NoImage:
        if (sz.x == 0 && sz.y == 0)
            cout << "Error: Button style specified NoImage but no size given\n";
        else {
            highlightTxtr.create(sz.x, sz.y);
            highlightTxtr.clear(Color(100,100,100,185));
        }
        text.setFont(Properties::SecondaryMenuFont);
        break;
    case Primary: {
            texture = imagePool.loadResource(Properties::MenuImagePath+"primaryBut.png");
            TextureReference tmp = imagePool.loadResource(Properties::MenuImagePath+"primaryButOver.png");
            Sprite spr(*tmp);
            highlightTxtr.create(tmp->getSize().x, tmp->getSize().y);
            highlightTxtr.draw(spr);
            text.setFont(Properties::PrimaryMenuFont);
        }
        break;
    case Secondary: {
            texture = imagePool.loadResource(Properties::MenuImagePath+"secondaryBut.png");
            TextureReference tmp = imagePool.loadResource(Properties::MenuImagePath+"secondaryButOver.png");
            Sprite spr(*tmp);
            highlightTxtr.create(tmp->getSize().x, tmp->getSize().y);
            highlightTxtr.draw(spr);
            text.setFont(Properties::SecondaryMenuFont);
        }
        break;
    default:
        cout << "Warning: Unrecognized button style: " << style << endl;
        break;
    }

    if (texture)
        image.setTexture(*texture);
    highlight.setTexture(highlightTxtr.getTexture());

    if (sz.x == 0 && sz.y == 0)
        size = Vector2f(image.getGlobalBounds().width, image.getGlobalBounds().height);
    else {
        size = sz;
        float xs = sz.x/texture->getSize().x;
        float ys = sz.y/texture->getSize().y;
        image.setScale(xs, ys);
        highlight.setScale(xs, ys);
    }

    text.setString(txt);
    setTextProperties(Color::Black, 14);
}

void MenuButton::setTextProperties(Color color, float fontSize, bool bold, bool italic) {
    Uint32 style = Text::Regular;
    if (bold)
        style |= Text::Bold;
    if (italic)
        style |= Text::Italic;
    text.setStyle(style);
    text.setFillColor(color);
    text.setCharacterSize(fontSize);
    textOffset.x = size.x/2 - text.getGlobalBounds().width/2;
    textOffset.y = size.y/2 - text.getGlobalBounds().height/2;
}

void MenuButton::p_render(RenderTarget& target, Vector2f pos) {
    image.setPosition(pos);
    highlight.setPosition(pos);
    text.setPosition(pos+textOffset);
    if (FloatRect(getPosition(), size).contains(Vector2f(Mouse::getPosition(Game::get().window))))
        target.draw(highlight);
    else
        target.draw(image);
    target.draw(text);
}
