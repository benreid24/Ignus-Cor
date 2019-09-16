#include "Game/Core/States/PauseState.hpp"
#include "Game/Core/Game.hpp"
#include "Game/Menu/GameMenu.hpp"
#include "Shared/Util/Timer.hpp"
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

PauseState::Ptr PauseState::create() {
    return Ptr(new PauseState());
}

PauseState::PauseState() : BaseState(nullptr) {
    startTime = Timer::get().timeElapsedRaw().asSeconds();
    resume = quit = false;
}

PauseState::~PauseState() {
    Timer::get().resume();
}

bool PauseState::doState() {
    Timer::get().pause();

    GameMenu menu(GameMenu::PauseMenu);
    menu.setComponentSpacing(20);

    MenuButton::Ptr resumeBut = MenuButton::create("Resume", MenuButton::Primary);
    MenuButton::Ptr quitBut = MenuButton::create("Quit", MenuButton::Secondary);

    resumeBut->setClickedCallback(makeMethodCallback<PauseState>(this, &PauseState::resumeCb));
    resumeBut->setTextProperties(Color::Black, 16, true);
    quitBut->setClickedCallback(makeMethodCallback<PauseState>(this, &PauseState::quitCb));
    quitBut->setTextProperties(Color::Black, 16, true);

    menu.addComponent(resumeBut);
    menu.addComponent(quitBut);

    while (true) {
        if (handleWindow())
            return true;
        if (runImmediate())
            return true;

        if (Keyboard::isKeyPressed(Keyboard::Return) && Timer::get().timeElapsedRaw().asSeconds()-startTime>0.75) {
            sleep(milliseconds(300));
            return false;
        }
        if (resume)
            return false;
        if (quit)
            return true;

        Game::get().window.clear();
        MapManager::get()->render();
        menu.render(Game::get().window);
        Game::get().window.display();
        ensureFps();
    }
}

void PauseState::resumeCb(const string&) {
    resume = true;
}

void PauseState::quitCb(const string&) {
    quit = true;
}
