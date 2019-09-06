#include "Shared/Entities/EntityBubble.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Properties.hpp"
using namespace sf;
using namespace std;

const Clock& EntityBubble::timer() {
    static Clock timer;
    return timer;
}

EntityBubble::EntityBubble(Mode m) {
	mode = m;
	bubbleTxtr = imagePool.loadResource(Properties::EntityBubbleImageFile);
	bubble.setTexture(*bubbleTxtr);
	nextId = 1;
}

int EntityBubble::addContent(const string& content, double length) {
    Content::Ptr c = Content::create(nextId++, content, length);
    contentQueue.push_back(c); //TODO - queue content addition to ensure primary thread loads resources
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
        position.y -= bubbleTxtr->getSize().y;
        Sprite sprite((*contentQueue.begin())->render().getTexture());
        sprite.setScale(1,-1);
        bubble.setPosition(position);
        sprite.setPosition(position+Vector2f(5,sprite.getGlobalBounds().height+5));
        target.draw(bubble);
        target.draw(sprite);
	}
}

void EntityBubble::update() {
    if (contentQueue.size() > 0) {
        Content::Ptr c = *contentQueue.begin();
        c->update(timer().getElapsedTime().asSeconds() - startTime);
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

/**
 * Class for displaying text in an EntityBubble
 */
class EntityBubble::TextContent : public EntityBubble::Content {
    static constexpr double ghostCharTime = 0.1;
    static constexpr int fontSize = 12;
    static constexpr int lineWidth = 28;

    string text;
    sf::Text renderText;
    sf::RenderTexture canvas;

public:
    /**
     * Creates the internal rendering buffer and word wraps the text
     */
    TextContent(int id, const string& txt, double length)
        : Content(id, length), text(txt) {
        renderText.setFont(Properties::ConversationFont);
        renderText.setCharacterSize(fontSize);
        renderText.setFillColor(Color::Black);

        int lastSpaceIndex = -1;
        int width = 1;
        for (unsigned int i = 1; i<text.size(); ++i) {
            if (text[i] == ' ')
                lastSpaceIndex = i;
            if (text[i] == '\n') {
                lastSpaceIndex = -1;
                width = 0;
            }
            if (width >= lineWidth) {
                if (lastSpaceIndex != -1) {
                    text[lastSpaceIndex] = '\n';
                    width = i - lastSpaceIndex;
                    lastSpaceIndex = -1;
                }
                else {
                    text.insert(i-1, "-\n");
                    width = 0;
                }
            }
            width += 1;
        }

        renderText.setString(text);
        canvas.create(renderText.getGlobalBounds().width+4, renderText.getGlobalBounds().height+4);
        renderText.setString("");
        renderText.setPosition(2, 2);
    }

    /**
     * Destructor
     */
    virtual ~TextContent() = default;

    /**
     * Returns true if the ghost writer is finished or the required time has elapsed
     */
    virtual bool finished() override {
        return renderText.getString().toAnsiString().size() == text.size();
    }

    /**
     * Updates the ghost writer
     */
    virtual void update(double timeAlive) override {
        const int chars = min(int(timeAlive / ghostCharTime), int(text.size()));
        renderText.setString(text.substr(0, chars));
    }

    /**
     * Renders the text and returns the RenderTexture
     */
    virtual const RenderTexture& render() override {
        canvas.clear(Color::Transparent);
        canvas.draw(renderText);
        return canvas;
    }
};

class EntityBubble::ImageContent : public EntityBubble::Content {
    TextureReference texture;
    Sprite sprite;
    RenderTexture canvas;

public:
    /**
     * Loads the image
     */
    ImageContent(int id, const string& file, double length)
        : Content(id, length) {
        texture = imagePool.loadResource(Properties::EntityBubbleImagePath+file);
        canvas.create(texture->getSize().x+4, texture->getSize().y+4);
        sprite.setTexture(*texture);
        sprite.setPosition(2, 2);
    }

    /**
     * Destructor
     */
    virtual ~ImageContent() = default;

    /**
     * Returns true
     */
    virtual bool finished() override {
        return true;
    }

    /**
     * Does nothing
     */
    virtual void update(double timeAlive) override {}

    /**
     * Renders the image and returns the RenderTexture
     */
    virtual const RenderTexture& render() override {
        canvas.clear(Color::Transparent);
        canvas.draw(sprite);
        return canvas;
    }
};

class EntityBubble::AnimContent : public EntityBubble::Content {
    AnimationReference animSrc;
    Animation anim;
    RenderTexture canvas;
    bool started;

public:
    /**
     * Loads the animation
     */
    AnimContent(int id, const string& file, double length)
        : Content(id, length) {
        started = false;
        animSrc = animPool.loadResource(Properties::EntityBubbleAnimPath+file);
        anim.setSource(animSrc);
        anim.setLooping(length < 0);

        Vector2f size(0,0);
        for (unsigned int i = 0; i<animSrc->numFrames(); ++i) {
            Vector2f ts = animSrc->getFrameSize(i);
            if (ts.x > size.x)
                size.x = ts.x;
            if (ts.y > size.y)
                size.y = ts.y;
        }

        canvas.create(size.x + 4, size.y + 4);
        anim.setPosition(Vector2f(2, 2));
    }

    /**
     * Destructor
     */
    virtual ~AnimContent() = default;

    /**
     * Returns true if the animation has played at least once
     */
    virtual bool finished() override {
        return started && !anim.isPlaying();
    }

    /**
     * Updates the animation
     */
    virtual void update(double timeAlive) override {
        if (!anim.isPlaying() && !started) {
            anim.play();
            started = true;
        }
        anim.update();
    }

    /**
     * Renders the image and returns the RenderTexture
     */
    virtual const RenderTexture& render() override {
        canvas.clear(Color::Transparent);

        Vector2f size = animSrc->getFrameSize(anim.getCurrentFrame());
        float x = canvas.getSize().x/2 - size.x/2;
        float y = canvas.getSize().y/2 - size.y/2;
        anim.setPosition(Vector2f(x, y));

        anim.draw(canvas);
        return canvas;
    }
};

EntityBubble::Content::Ptr EntityBubble::Content::create(int id, const string& str, double len) {
    const string ext = File::getExtension(str);
    if (str == "anim")
        return Ptr(new EntityBubble::AnimContent(id, str, len));
    else if (str == "png" || str == "jpg")
        return Ptr(new EntityBubble::ImageContent(id, str, len));
    return Ptr(new EntityBubble::TextContent(id, str, len));
}
