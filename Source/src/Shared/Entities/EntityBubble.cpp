#include "Shared/Entities/EntityBubble.hpp"
using namespace sf;
using namespace std;

const Clock& EntityBubble::timer() {
    static Clock timer;
    return timer;
}

EntityBubble::EntityBubble(Mode m) {
	mode = m;
	//TODO - load bubble texture
	spr.setTexture(txtr.getTexture());
	nextId = 1;
}

int EntityBubble::addContent(const string& content, double length) {
    Content::Ptr c = Content::create(nextId++, content, length);
    contentQueue.push_back(c); //TODO - thread safe?
    if (contentQueue.size() == 1)
        startTime = EntityBubble::timer().getElapsedTime().asSeconds();
    return c->getId();
}

EntityBubble::ContentStatus EntityBubble::queryContent(int id) {
    for (auto i = contentQueue.begin(); i!=contentQueue.end(); ++i) {
        if (id == (*i)->getId()) {
            if (i == contentQueue.begin())
                return Displaying;
            return Queued;
        }
    }
    return Finished;
}

int EntityBubble::queryContentSpecificStatus(int id) {
    for (auto i = contentQueue.begin(); i!=contentQueue.end(); ++i) {
        if (id == (*i)->getId()) {
            return (*i)->getStatus();
        }
    }
    return -1;
}

bool EntityBubble::removeContent(int id) {
    for (auto i = contentQueue.begin(); i!=contentQueue.end(); ++i) {
        if (id == (*i)->getId()) {
            contentQueue.erase(i);
            return true;
        }
    }
    return false;
}

void EntityBubble::setMode(Mode m) {
    mode = m;
}

void EntityBubble::render(sf::RenderTarget& target, sf::Vector2f position) {
	if (contentQueue.size() > 0) {
        RenderTexture& texture = (*contentQueue.begin())->render();
        Sprite sprite(texture.getTexture());
        //TODO - scale to proper size and perspective
        spr.setPosition(position);
        sprite.setPosition(position);
        target.draw(spr);
        target.draw(sprite);
	}
}

void EntityBubble::update() {
    if (contentQueue.size() > 0) {
        Content::Ptr c = *contentQueue.begin();
        if (c->finished()) {
            if (timer().getElapsedTime().asSeconds() - startTime >= c->getLength() && c->getLength() >= 0) {
                if (mode == Loop)
                    contentQueue.push_back(c);
                contentQueue.pop_front();
                startTime = timer().getElapsedTime().asSeconds();
            }
        }
    }
}
