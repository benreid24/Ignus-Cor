#ifndef EVENTOBSERVER_HPP
#define EVENTOBSERVER_HPP

#include <SFML/Window.hpp>
#include <memory>

/**
 * Virtual base class for objects that can subscribe to window events
 *
 * \ingroup Game
 */
class EventObserver {
public:
    typedef std::shared_ptr<EventObserver> Ptr;
    typedef std::weak_ptr<EventObserver> WeakPtr;

    /**
     * Pure virtual
     */
    virtual ~EventObserver() = default;

    /**
     * Observe the event
     */
    virtual void observe(const sf::Event& event) = 0;

    /**
     * Notify that a new queue of events is lined up. Called at beginning of update frame
     */
    virtual void notifyFrameStart() {}
};

#endif // EVENTOBSERVER_HPP
