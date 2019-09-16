#include <Game/Core/EventDispatcher.hpp>
using namespace std;
using namespace sf;

void EventDispatcher::addObserver(EventObserver::WeakPtr o) {
    transientObservers.emplace_back(o);
}

void EventDispatcher::addGlobalObserver(EventObserver* o) {
    globalObservers.push_back(o);
}

void EventDispatcher::dispatch(const Event& e) {
    for (unsigned int i = 0; i<globalObservers.size(); ++i) {
        globalObservers[i]->observe(e);
    }
    list<list<EventObserver::WeakPtr>::iterator> invalid;
    for (auto i = transientObservers.begin(); i != transientObservers.end(); ++i) {
        EventObserver::Ptr o = i->lock();
        if (o)
            o->observe(e);
        else
            invalid.push_back(i);
    }
    for (auto i = invalid.begin(); i!=invalid.end(); ++i) {
        transientObservers.erase(*i);
    }
}

void EventDispatcher::notifyFrameStart() {
    for (unsigned int i = 0; i<globalObservers.size(); ++i) {
        globalObservers[i]->notifyFrameStart();
    }
    list<list<EventObserver::WeakPtr>::iterator> invalid;
    for (auto i = transientObservers.begin(); i != transientObservers.end(); ++i) {
        EventObserver::Ptr o = i->lock();
        if (o)
            o->notifyFrameStart();
        else
            invalid.push_back(i);
    }
    for (auto i = invalid.begin(); i!=invalid.end(); ++i) {
        transientObservers.erase(*i);
    }
}
