#include "Game/Core/BaseState.hpp"
#include "Game/Core/Game.hpp"
using namespace std;

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
