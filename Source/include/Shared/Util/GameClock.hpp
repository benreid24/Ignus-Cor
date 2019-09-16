#ifndef GAMECLOCK_HPP
#define GAMECLOCK_HPP

#include "Shared/Util/Timer.hpp"

/**
 * Structure for storing the game time in hour:minute form
 *
 * \ingroup Utilities
 */
struct ClockTime
{
    int hour, minute;

    /**
     * Initializes the time to noon (12:00)
     */
    ClockTime()
    {
        hour = 12;
        minute = 0;
    }

    /**
     * Initializes the time to the given hour and minute
     *
     * \param h The hour of the time [0,23]
     * \param m The minute of the time [0,59]
     */
    ClockTime(int h, int m)
    {
        hour = h;
        minute = m;
    }
};

/**
 * This class is responsible for initializing, storing and updating the game time, both as time elapsed since play start and clock time in the game
 *
 * \ingroup Utilities
 */
class GameClock
{
    ClockTime start;

public:
    /**
     * Starts the time noon
     */
    GameClock(): start(12,0) {}

    /**
     * Returns the simulated in game time
     */
    ClockTime getClockTime() //present scale: 3 seconds = 1 minute
    {
        ClockTime t = start;
        float s = Timer::get().timeElapsedSeconds();
        t.hour += int(s/3.0+(float)t.minute)/60)%24;
        t.minute += int(s/3.0)%60;

        return t;
    }

    /**
     * Sets the internal simulated time of the game
     *
     * \param t The time to set the game clock to
     */
    void setClockTime(ClockTime t)
    {
        start = t;
    }
};

#endif // GAMECLOCK_HPP
