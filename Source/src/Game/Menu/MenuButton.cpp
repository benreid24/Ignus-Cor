#include "Game/Menu/MenuButton.hpp"
#include "Shared/Properties.hpp"
using namespace sf;
using namespace std;

MenuButton::Ptr MenuButton::create(const string& text, Style style, Vector2f size, float fontSize) {
    return Ptr(new MenuButton(text, style, size, fontSize));
}

MenuButton::MenuButton(const string& txt, Style style, Vector2f sz, float fontSize)
: MenuComponent(txt) {
    switch (style) {
    case NoImage:
        if (sz.x == 0 && sz.y == 0)
            cout << "Error: Button style specified NoImage but no size given\n";
        else {
            highlightTxtr.create(sz.x, sz.y);
            highlightTxtr.clear(Color(100,100,100,185));
        }
        break;
    case Primary: {
            texture = imagePool.loadResource(Properties::MenuImagePath+"primaryBut.png");
            TextureReference tmp = imagePool.loadResource(Properties::MenuImagePath+"primaryButOver.png");
            Sprite spr(*tmp);
            highlightTxtr.create(tmp->getSize().x, tmp->getSize().y);
            highlightTxtr.draw(spr);
        }
        break;
    case Secondary: {
            texture = imagePool.loadResource(Properties::MenuImagePath+"secondaryBut.png");
            TextureReference tmp = imagePool.loadResource(Properties::MenuImagePath+"secondaryButOver.png");
            Sprite spr(*tmp);
            highlightTxtr.create(tmp->getSize().x, tmp->getSize().y);
            highlightTxtr.draw(spr);
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
    text.setCharacterSize(fontSize>0 ? fontSize : 14);
    textOffset.x = size.x/2 - text.getGlobalBounds().width/2;
    textOffset.y = size.y/2 - text.getGlobalBounds().height/2;
}

void MenuButton::p_render(RenderTarget& target, Vector2f pos) {
    image.setPosition(pos);
    highlight.setPosition(pos);
    text.setPosition(pos+textOffset);
    if (FloatRect(getPosition(), size).contains(Vector2f(Mouse::getPosition())))
        target.draw(highlight);
    else
        target.draw(image);
    target.draw(text);
}
