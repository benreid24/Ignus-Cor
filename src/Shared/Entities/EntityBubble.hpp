#ifndef ENTITYBUBBLE_HPP
#define ENTITYBUBBLE_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <string>
#include "Shared/Util/ResourcePool.hpp"
#include "Shared/Media/Animation.hpp"

/**
 * Helper class for displaying thought bubbles near entities. Bubbles can contain text, images,
 * and animations. Content can be
 *
 * \ingroup Entity
 */
class EntityBubble {
public:
	/**
	 * Display mode of the bubble. DisplayOnce causes content to be deleted once it renders, loop will loop through content forever
	 */
	enum Mode {
		DisplayOnce,
		Loop
	};

	/**
	 * Helper enum for querying content status
	 */
	enum ContentStatus {
        Queued,
        Displaying,
        Finished
	};

	/**
	 * Enum to represent if content should persist based on time or player input
	 */
    enum PersistType {
        Input,
        Time
    };

	/**
	 * Basic constructor
	 */
	EntityBubble(Mode mode = DisplayOnce);

    /**
     * Adds the given content to the bubble
     *
     * \param content String representation of content. Can be animation, image, or text
     * \param length Length of time to display content in seconds. 0 = ghost writer/anim length. -1 = forever
     * \return An id that can be used to track the status of the content
     */
	int addContent(const std::string& content, PersistType type = Time, double length = 0);

	/**
	 * Terminates the content with the given id
	 *
	 * \return True if content was removed, false otherwise
	 */
    bool removeContent(int id);

	/**
	 * Returns the status of the content with the given id. Returns Finished if the id is invalid
	 */
    ContentStatus queryContent(int id);

    /**
     * Returns content type specific status. Used mainly for returning the chosen choice of a prompt
     */
    int queryContentSpecificStatus(int id);

	/**
	 * Sets the mode
	 */
	void setMode(Mode mode);

	/**
	 * Updates the bubble
	 */
	void update();

	/**
	 * Renders the bubble
	 *
	 * \param target The target to draw to
	 * \param position The on screen position of the Entity it is associated with
	 */
	void render(sf::RenderTarget& target, sf::Vector2f position); //note, scale vertical down for visual effect

private:
    /**
     * Base class for representing content
     */
    class Content {
        int id;
        EntityBubble::PersistType pType;
        double length;

    public:
        typedef std::shared_ptr<Content> Ptr;

        /**
         * Creates an appropriate content sub class and returns pointer to base
         */
        static Ptr create(int id, const std::string& content, EntityBubble::PersistType pt, double length);

        /**
         * Creates the content with the given id and length
         */
        Content(int i, EntityBubble::PersistType pt, double l) : id(i), pType(pt), length(l) {}

        /**
         * Destructor
         */
        virtual ~Content() = default;

        /**
         * Returns the id
         */
        int getId() { return id; }

        /**
         * Returns the length
         */
        double getLength() { return length; }

        /**
         * Returns the persistence type
         */
        EntityBubble::PersistType persistType() { return pType; }

        /**
         * Returns true when finished displaying. Does not account for specified display time
         */
        virtual bool finished() = 0;

        /**
         * Returns type specific status info
         */
        virtual int getStatus() { return -1; }

        /**
         * Updates internal content
         */
        virtual void update(double timeAlive) = 0;

        /**
         * Skip any slow displaying mechanism. Player clicked it
         */
        virtual void fastForward() = 0;

        /**
         * Renders to an internal RenderTexture and returns a reference to it
         */
        virtual const sf::RenderTexture& render() = 0;
    };
    class TextContent;
    class ImageContent;
    class AnimContent;
    class OptionContent;

private:
	TextureReference bubbleTxtr;
	sf::Sprite bubble;

	std::list<Content::Ptr> contentQueue;
	double startTime, finishTime;
	int nextId;

	enum State {
		Inactive,
		DisplayingContent,
		FadingIn,
		FadingOut
	}state;
	Mode mode;

	/**
	 * Helper function to tell if the bubble was clicked
	 */
    bool clicked();
};

#endif
