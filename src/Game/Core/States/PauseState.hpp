#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "Game/Core/BaseState.hpp"

/**
 * State for when the game is paused
 *
 * \ingroup Core
 */
class PauseState : public BaseState {
public:
    typedef std::shared_ptr<PauseState> Ptr;

    /**
     * Creates a PauseState
     */
    static Ptr create();

    /**
     * Unpauses the global timer
     */
    virtual ~PauseState();

private:
    float startTime;
    bool resume, quit;

    /**
     * Actually create the state
     */
    PauseState();

    /**
     * Runs the pause menu
     */
    virtual bool doState() override;

    /**
     * Callback for the resume button
     */
    void resumeCb(const std::string&);

    /**
     * Callback for the quit button
     */
    void quitCb(const std::string&);
};

#endif // PAUSESTATE_HPP
