#include "Game/Core/States/PauseState.hpp"
#include "Game/Core/Game.hpp"
#include "Shared/Util/Timer.hpp"
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

PauseState::Ptr PauseState::create() {
    return Ptr(new PauseState());
}

PauseState::PauseState() : BaseState(nullptr) {
    startTime = Timer::get().timeElapsedRaw().asSeconds();
}

PauseState::~PauseState() {
    Timer::get().resume();
}

bool PauseState::doState() {
    //make the menu
    Text text;
    text.setFont(Properties::PrimaryMenuFont);
    text.setString("PAUSED");
    text.setFillColor(Color::Black);
    text.setOutlineColor(Color::Red);
    text.setOutlineThickness(2);
    text.setCharacterSize(46);
    text.setPosition(120,100);

    Timer::get().pause();
    while (true) {
        if (handleWindow())
            return true;
        if (runImmediate())
            return true;

        if (Keyboard::isKeyPressed(Keyboard::Return) && Timer::get().timeElapsedRaw().asSeconds()-startTime>0.75) {
            sleep(milliseconds(300));
            return false;
        }

        Game::get().window.clear();
        MapManager::get()->render();
        Game::get().window.draw(text);
        Game::get().window.display();
        ensureFps();
    }
}
