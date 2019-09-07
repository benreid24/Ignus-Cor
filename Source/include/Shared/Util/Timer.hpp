#ifndef TIMER_HPP
#define TIMER_HPP

#include <SFML/System.hpp>

/**
 * Global class for tracking time. Implemented over the top of sf::Clock
 * to allow for pausing
 *
 * \ingroup Util
 */
class Timer {
public:
    /**
     * Returns the global timer object
     */
    static Timer& get();

    /**
     * Returns the time elapsed in seconds
     */
    float timeElapsedSeconds() const;

    /**
     * Returns the time elapsed in milliseconds
     */
    unsigned long int timeElapsedMilliseconds() const;

    /**
     * Returns the raw SFML elapsed time
     */
    sf::Time timeElapsed() const;

    /**
     * Pauses the timer
     */
    void pause();

    /**
     * Resumes the timer
     */
    void resume();

private:
    /**
     * Starts the timer
     */
    Timer();

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

    sf::Clock clock;
    sf::Time timePaused, offset;
    bool paused;
};

#endif // TIMER_HPP
