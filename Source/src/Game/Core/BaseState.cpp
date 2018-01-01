#include "Game/Core/BaseState.hpp"
#include "Game/Core/Game.hpp"
#include "Shared/Properties.hpp"
using namespace std;
using namespace sf;

const float BaseState::fpsLimit = 60;
const float BaseState::frameLength = 1000.0/BaseState::fpsLimit;
float BaseState::lastUpdateTime = 0;
Clock BaseState::timer;

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

BaseState::BaseState(Game* g, BaseState::Ptr next) {
	game = g;
	nextState = next;
}

void BaseState::setNextState(BaseState::Ptr next) {
	nextState = next;
}

bool BaseState::start() {
	game->states.push(this);
	bool r = doState();
	game->states.pop();
	if (!r && nextState)
		nextState->start();
	return r;
}

void BaseState::runState(BaseState::Ptr immediate) {
	immediateState = immediate;
}

void BaseState::runImmediate() {
	if (immediateState) {
		immediateState->start();
		immediateState.reset();
	}
}

void BaseState::ensureFps() {
    float dTime = timer.getElapsedTime().asSeconds()-lastUpdateTime;
    int waitTime = frameLength-dTime*1000;
    if (waitTime>0)
        sleep(milliseconds(waitTime));
    lastUpdateTime = timer.getElapsedTime().asSeconds();
}

bool BaseState::handleWindow() {
    Event event;
    while (game->window.pollEvent(event)) {
        if (event.type==Event::Closed) {
            return true;
        }
        if (event.type==Event::Resized) {
			View view = getView(event.size.width,event.size.height);
			game->window.setView(view);
		}
		if (event.type==Event::GainedFocus)
			game->inFocus = true;
		if (event.type==Event::LostFocus)
			game->inFocus = false;
    }
    return false;
}
