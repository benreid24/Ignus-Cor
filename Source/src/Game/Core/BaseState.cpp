#include "Game/Core/BaseState.hpp"
#include "Game/Core/Game.hpp"
#include "Game/Core/PlayerInput.hpp"
#include "Shared/Properties.hpp"
#include "Shared/DebugOverlays.hpp"
#include "Shared/Util/Timer.hpp"
using namespace std;
using namespace sf;

const float BaseState::fpsLimit = 60;
const float BaseState::frameLength = 1.0/BaseState::fpsLimit;
float BaseState::lastUpdateTime = 0;

namespace {
    View getView(float w, float h)
    {
        View view(FloatRect(0,0,Properties::ScreenWidth,Properties::ScreenHeight));
        float newWidth = w;
        float newHeight = h;
        float xScale = newWidth/float(Properties::ScreenWidth);
        float yScale = newHeight/float(Properties::ScreenHeight);
        float xView = 1;
        float yView = 1;
        float xBase = 0;
        float yBase = 0;

        if (xScale>yScale) //keep y scale
        {
            xView = Properties::ScreenWidth*yScale/newWidth;
            xBase = (1-xView)/2;
        }
        else
        {
            yView = Properties::ScreenHeight*xScale/newHeight;
            yBase = (1-yView)/2;
        }

        view.setViewport(FloatRect(xBase,yBase,xView,yView));
        return view;
    }
}

BaseState::BaseState(BaseState::Ptr next) {
	nextState = next;
	debugToggledTime = 0;
}

void BaseState::setNextState(BaseState::Ptr next) {
	nextState = next;
}

bool BaseState::start() {
	Game::get().states.push(this);
	bool r = doState();
	Game::get().states.pop();
	if (!r && nextState)
		nextState->start();
	return r;
}

void BaseState::runState(BaseState::Ptr immediate) {
	if (immediateState)
        cout << "Warning: runState() called on state but another state already queued up\n";
	immediateState = immediate;
}

bool BaseState::runImmediate() {
	if (immediateState) {
		bool r = immediateState->start();
		immediateState.reset();
		return r;
	}
	return false;
}

void BaseState::ensureFps() {
    float dTime = Timer::get().timeElapsedRaw().asSeconds()-lastUpdateTime;
    int waitTime = frameLength-dTime;
    if (waitTime>0)
        sleep(seconds(waitTime));
    lastUpdateTime = Timer::get().timeElapsedRaw().asSeconds();
}

bool BaseState::handleWindow() {
    Game::get().eventDispatcher.notifyFrameStart();

    Event event;
    while (Game::get().window.pollEvent(event)) {
        if (event.type==Event::Closed) {
            return true;
        }
        if (event.type==Event::Resized) {
			View view = getView(event.size.width,event.size.height);
			Game::get().window.setView(view);
		}
		if (event.type==Event::GainedFocus)
			Game::get().inFocus = true;
		if (event.type==Event::LostFocus)
			Game::get().inFocus = false;

        Game::get().eventDispatcher.dispatch(event);
    }

    if (Keyboard::isKeyPressed(Keyboard::Numpad1))
        DebugOverlays::toggleOverlay(DebugOverlays::EntityInfo);
    if (Keyboard::isKeyPressed(Keyboard::Numpad2))
        DebugOverlays::toggleOverlay(DebugOverlays::CombatData);
    if (Keyboard::isKeyPressed(Keyboard::Numpad0))
        DebugOverlays::toggleOverlay(DebugOverlays::BoundingBoxes);

    return false;
}
