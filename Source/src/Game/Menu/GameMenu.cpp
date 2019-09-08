#include "Game/Menu/GameMenu.hpp"
#include "Shared/Properties.hpp"
using namespace std;
using namespace sf;

GameMenu::GameMenu(Type type) {
    MenuBox::PackDirection packDir = MenuBox::Vertical;
    switch (type) {
    case PauseMenu:
        txtr = imagePool.loadResource(Properties::MenuImagePath+"pauseBgnd.png");
        position.x = Properties::ScreenWidth/2 - txtr->getSize().x/2;
        position.y = Properties::ScreenHeight/2 - txtr->getSize().y/2;
        break;

    default:
        cout << "Error: Unrecognized GameMenu type: " << type << endl;
        break;
    }

    background.setTexture(*txtr);
    Vector2f size(background.getGlobalBounds().width, background.getGlobalBounds().height);
    box = MenuBox::create(size, packDir);
}

GameMenu::GameMenu(const string& bgnd, MenuBox::PackDirection dir) {
    txtr = imagePool.loadResource(Properties::MenuImagePath+bgnd);
    background.setTexture(*txtr);
    Vector2f size(background.getGlobalBounds().width, background.getGlobalBounds().height);
    box = MenuBox::create(size, dir);
}

GameMenu::GameMenu(Vector2f size, MenuBox::PackDirection dir) {
    box = MenuBox::create(size, dir);
}

void GameMenu::addComponent(MenuComponent::Ptr component) {
    box->pack(component);
}

void GameMenu::setPosition(Vector2f pos) {
    position = pos;
}

void GameMenu::render(RenderTarget& target) {
    box->render(target, position);
}
