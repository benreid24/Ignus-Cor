#ifndef MAINSTATE_HPP
#define MAINSTATE_HPP

#include "Game/Core/BaseState.hpp"

/**
 * Main game state. Renders the map and all Entities. Update everything. Spawns sub states as needed
 *
 * \ingroup Core
 */
class MainState : public BaseState {
public:
    typedef std::shared_ptr<MainState> Ptr;

private:
    /**
     * Runs the state
     *
     * \return True if the game should close, false otherwise
     */
    virtual bool doState() override;

    /**
     * Creates the state and all necessary data
     */
    MainState();

public:
    /**
     * Creates and returns a pointer to a new MainState
     */
    static Ptr create();
};

#endif // MAINSTATE_HPP
