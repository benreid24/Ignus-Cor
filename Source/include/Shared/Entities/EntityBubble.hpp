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
	 * Basic constructor
	 */
	EntityBubble(Mode mode = DisplayOnce);

    /**
     * Adds the given content to the bubble
     *
     * \param content String representation of content. Can be animation, image, or text
     * \param length Length of time to display content in ms. 0 = ghost writer/anim length. -1 = forever
     * \return An id that can be used to track the status of the content
     */
	int addContent(const std::string& content, int length = 0);

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
     * Helper struct for EntityBubble to store content
     */
    struct EntityBubbleContent { //TODO - convert to base class and provide manipulators for bubble
        enum Type {
            Text,
            Image,
            Anim
        }type;

        TextureReference txtr;
        AnimationReference animSrc;

        sf::Sprite spr;
        Animation anim;
        std::string text;

        int id;
        int length;

        /**
         * Creates the bubble content from the given string. This function will determine if the
         * string is an image file, animation file, or just text
         *
         * \param init The string to load from
         * \param length How long, in milliseconds, to display. Pass 0 to stop when animation finishes or wait for ghost writer, or -1 to persist forever
         */
        EntityBubbleContent(const std::string& init, int length);
    };

private:
	sf::RenderTexture txtr;
	sf::Sprite spr;

	static sf::Clock timer;
	std::list<EntityBubbleContent> contentQueue;
	int nextId;
	int startTime;

	enum State {
		Inactive,
		DisplayingContent,
		FadingIn,
		FadingOut
	}state;
	Mode mode;
};

#endif
