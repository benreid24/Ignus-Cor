#ifndef ENTITYBUBBLE_HPP
#define ENTITYBUBBLE_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Shared/Util/ResourcePool.hpp"
#include "Shared/Media/Animation.hpp"

class EntityBubble;

/**
 * Helper class for EntityBubble to store content
 */
class EntityBubbleContent {
	enum Type {
		Text,
		Image,
		Anim
	}type;

	TextureReference txtr;
	AnimationSource animSrc;

	sf::Sprite spr;
	Animation anim;
	std::string text;

	friend class EntityBubble;

public:
	/**
	 * Creates the bubble content from the given string. This function will determine if the
	 * string is an image file, animation file, or just text
	 *
	 * \param init The string to load from
	 * \param length How long, in milliseconds, to display. Pass 0 to stop when animation finishes, or -1 to persist forever
	 */
	EntityBubbleContent(std::string init, int length);
};

/**
 * Helper class for displaying thought bubbles near entities. Bubbles can contain text, images,
 * and animations. Content can be
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

private:
	sf::RenderTexture txtr;
	sf::Sprite spr;

	static sf::Clock timer;
	std::vector<EntityBubbleContent> content;
	int startTime;

	enum State {
		Inactive,
		DisplayingContent,
		FadingIn,
		FadingOut
	}state;
	Mode mode;

public:
	/**
	 * Basic constructor
	 */
	EntityBubble(Mode mode = DisplayOnce);

    /**
     * Adds the given content to the bubble
     */
	void addContent(std::string content, int length);

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
};

#endif
