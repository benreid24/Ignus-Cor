#ifndef BASESTATE_HPP
#define BASESTATE_HPP

#include <memory>
#include <SFML/System.hpp>

class Game;

/**
 * Base class for states. Provides some basic functionality but leaves most work to child classes
 *
 * \ingroup Core
 */
class BaseState {
    int debugToggledTime;

public:
	typedef std::shared_ptr<BaseState> Ptr;

protected:
	BaseState::Ptr nextState;
	BaseState::Ptr immediateState;

	/**
	 * Actually runs the state. Note: This should never be ran directly.
	 *
	 * \return True if the game should close, false otherwise
	 */
	virtual bool doState() = 0;

	/**
	 * Checks the immediateState flag and executes it if present. Children states should call this once per loop
	 */
	void runImmediate();

	/**
	 * Constructs the state and optionally sets the state to run after this state finishes
	 */
	BaseState(BaseState::Ptr next);

	/**
	 * Handles window events
	 */
    bool handleWindow();

    /**
     * Ensures that the game runs at exactly 60 fps, no more (but maybe less)
     */
    static void ensureFps();

    static const float fpsLimit, frameLength;
    static sf::Clock timer;
    static float lastUpdateTime;

public:
	/**
	 * Constructs the state and optionally sets the state to run after this state finishes
	 */
	static Ptr create(BaseState::Ptr next = nullptr);

	/**
	 * Sets the state to run upon completion of the current state
	 */
	void setNextState(BaseState::Ptr next);

	/**
	 * Immediately runs the given state, and completes the current state after the immediate state has finished
	 *
	 * This is implemented by setting an internal flag which is then checked every update loop
	 * Some children may not do this, resulting in the state never being executed
	 * It is meant as a means for non state code to run new states
	 */
	void runState(BaseState::Ptr immediate);

	/**
	 * Call this to properly execute the state
	 */
	bool start();
};

#endif // BASESTATE_HPP
