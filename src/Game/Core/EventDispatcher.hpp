#ifndef EVENTDISPATCHER_HPP
#define EVENTDISPATCHER_HPP

#include "Game/Core/EventObserver.hpp"
#include <list>
#include <vector>

class Game;

/**
 * Utility class to dispatch events to observers and to manage observer queue
 *
 * \ingroup Game
 */
class EventDispatcher {
public:
    /**
     * Constructs the dispatcher with empty observer queues
     */
    EventDispatcher() = default;

    /**
     * Destructor
     */
    ~EventDispatcher() = default;

    /**
     * Adds an observer to the queue
     */
    void addObserver(EventObserver::WeakPtr observer);

    /**
     * Dispatches an event to all of the observers
     */
    void dispatch(const sf::Event& event);

    /**
     * Notifies all observers of a frame start
     */
    void notifyFrameStart();

private:
    std::vector<EventObserver*> globalObservers;
    std::list<EventObserver::WeakPtr> transientObservers;

    /**
     * Privileged function for adding global observers. These are
     * guaranteed to always be valid
     */
    void addGlobalObserver(EventObserver* observer);

    friend class Game;
};

#endif // EVENTDISPATCHER_HPP
