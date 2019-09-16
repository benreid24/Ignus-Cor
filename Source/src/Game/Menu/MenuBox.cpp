#include "Game/Menu/MenuBox.hpp"
#include "Shared/Properties.hpp"
using namespace std;
using namespace sf;

MenuBox::MenuBox(PackDirection dir)
: MenuComponent("MenuBox")
, scrollFactor(2.0)
, scrollBarWidth(12)
, scrollChannelThickness(2)
, minScrollbarSize(12)
, scrollSizeFactor(1.0) {
    packDir = dir;
    scrollingEnabled = false;
    totalSize = scrollPos = packBuffer = 0;
    scrollbar.setFillColor(Color(115, 117, 120));
    scrollChannel.setFillColor(Color(178, 180, 184, 128));
    scrollChannel.setOutlineColor(Color(51, 51, 51, 200));
    scrollChannel.setOutlineThickness(scrollChannelThickness);
}

MenuBox::MenuBox(const string& img, PackDirection dir)
: MenuBox(dir) {
    bgndTxtr = imagePool.loadResource(Properties::MenuImagePath+img);
    background.setTexture(*bgndTxtr);
    size.x = background.getGlobalBounds().width;
    size.y = background.getGlobalBounds().height;
    boxTarget.create(size.x, size.y);
    box.setTexture(boxTarget.getTexture());
    box.setScale(1, -1);
}

MenuBox::MenuBox(Vector2f sz, PackDirection dir)
: MenuBox(dir) {
    size = sz;
    boxTarget.create(size.x, size.y);
    box.setTexture(boxTarget.getTexture());
    box.setScale(1, -1);
}

MenuBox::Ptr MenuBox::create(const string& img, PackDirection dir) {
    return Ptr(new MenuBox(img, dir));
}

MenuBox::Ptr MenuBox::create(Vector2f size, PackDirection dir) {
    return Ptr(new MenuBox(size, dir));
}

void MenuBox::setPackBuffer(float buffer) {
    packBuffer = buffer;
    computeSize();
}

void MenuBox::setScrollingEnabled(bool scroll) {
    scrollingEnabled = scroll;
}

void MenuBox::pack(MenuComponent::Ptr component) {
    children.push_back(component);
    computeSize();
}

void MenuBox::computeSize() {
    totalSize = packBuffer;
    if (packDir == Vertical) {
        for (unsigned int i = 0; i<children.size(); ++i) {
            totalSize += children[i]->getSize().y;
            if (i < children.size()-1)
                totalSize += packBuffer;
        }
        if (totalSize > size.y && !scrollingEnabled) {
            cout << "Warning: Box contents exceed size but scrolling is disabled!\n";
        }
    }
    else {
        for (unsigned int i = 0; i<children.size(); ++i) {
            totalSize += children[i]->getSize().x;
            if (i < children.size()-1)
                totalSize += packBuffer;
        }
        if (totalSize > size.x && !scrollingEnabled) {
            cout << "Warning: Box contents exceed size but scrolling is disabled!\n";
        }
    }
}

void MenuBox::p_render(RenderTarget& target, Vector2f pos) {
    Vector2f cpos(packBuffer, packBuffer);
    float& z = (packDir == Horizontal) ? cpos.x : cpos.y;
    const float maxPos = (packDir == Horizontal) ? size.x : size.y;
    Vector2f offset;
    if (packDir == Horizontal)
        offset.x = scrollPos;
    else
        offset.y = scrollPos;

    boxTarget.clear(Color::Transparent);
    if (bgndTxtr)
        boxTarget.draw(background);

    for (unsigned int i = 0; i<children.size(); ++i) {
        float s = 0;
        if (packDir == Horizontal) {
            s = children[i]->getSize().x;
            cpos.y = size.y/2 - children[i]->getSize().y/2;
        }
        else {
            s = children[i]->getSize().y;
            cpos.x = size.x/2 - children[i]->getSize().x/2;
        }
        if (z+s >= scrollPos && z <= scrollPos+maxPos) {
            children[i]->render(boxTarget, cpos-offset, pos+cpos-offset);
        }
        z += s + packBuffer;
    }
    if (totalSize > maxPos) {
        if (packDir == Horizontal) {
            scrollChannel.setSize(
                Vector2f(size.x-scrollChannelThickness*2,
                         scrollBarWidth
                )
            );
            scrollChannel.setPosition(
                pos.x+scrollChannelThickness,
                pos.y+size.y-scrollChannelThickness*2
            );

            float barSize = size.x/totalSize * scrollSizeFactor;
            if (barSize < minScrollbarSize)
                barSize = minScrollbarSize;
            scrollbar.setSize(Vector2f(barSize, scrollBarWidth));

            float portion = scrollPos/totalSize;
            float freeSpace = size.x - barSize;
            scrollbar.setPosition(
                pos.x+scrollChannelThickness+freeSpace*portion,
                pos.y+size.y-scrollBarWidth-scrollChannelThickness
            );
        }
        else {
            scrollChannel.setSize(
                Vector2f(scrollBarWidth,
                         size.y-scrollChannelThickness*2
                )
            );
            scrollChannel.setPosition(
                pos.x+size.x-scrollBarWidth-scrollChannelThickness*2,
                pos.y+scrollChannelThickness
            );

            float barSize = size.y/totalSize * scrollSizeFactor;
            if (barSize < minScrollbarSize)
                barSize = minScrollbarSize;
            scrollbar.setSize(Vector2f(scrollBarWidth, barSize));

            float portion = scrollPos/totalSize;
            float freeSpace = size.y - barSize;
            scrollbar.setPosition(
                pos.x+size.x-scrollBarWidth-scrollChannelThickness,
                pos.y+scrollChannelThickness+freeSpace*portion
            );
        }
    }
    box.setPosition(pos.x, pos.y + boxTarget.getSize().y);
    target.draw(box);
}

void MenuBox::p_observe(const Event& e) {
    if (e.type == Event::MouseWheelScrolled) {
        FloatRect bounds(getPosition(), size);
        if (bounds.contains(e.mouseWheelScroll.x, e.mouseWheelScroll.y)) {
            scrollPos += e.mouseWheelScroll.delta * scrollFactor;
            float sz = packDir == Horizontal ? size.x : size.y;
            if (scrollPos < 0)
                scrollPos = 0;
            if (scrollPos > totalSize-sz)
                scrollPos = totalSize-sz;
        }
    }
}
