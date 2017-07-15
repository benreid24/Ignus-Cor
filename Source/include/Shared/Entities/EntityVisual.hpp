#ifndef ENTITYVISUAL_HPP
#define ENTITYVISUAL_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include "Shared/Util/ResourcePool.hpp"
#include "Shared/Media/Animation.hpp"

/**
 * Abstraction class to handle rendering of entities. Can handle static images, single animations,
 * direction based animations and speed+direction based animations
 */
class EntityVisual {
	AnimationReference slowSrc[4], fastSrc[4];
	Animation slow[4], fast[4];
	TextureReference txtr;
	sf::Sprite image;
	int dir;

	enum Type {
		NotLoaded,
		StaticImage,
		SingleAnim,
		MoveAnim,
		SpeedAnim
	}type;

	enum State {
		Still,
		Moving,
		FastMoving
	}state;

public:
	/**
	 * Basic constructor
	 */
	EntityVisual();

	/**
	 * Constructs the object from the given paths. Will determine based on the file extensions whether it
	 * is a static image, single animation, moving animations, or speed based moving animations
	 *
	 * \param path1 The path to the image, animation, or directory containing the regular speed animations
	 * \param path2 Optional path to a directory containing the fast animations
	 */
	void load(std::string path1, std::string path2 = "");

	/**
	 * Sets the state to be moving in the given direction and speed. If this is not called before being rendered,
	 * the state is reset to being still, but the direction persists
	 *
	 * \param dir The direction to face
	 * \param fast Whether to move fast or regular
	 */
	void setMoving(int dir, bool fast = false);

	/**
	 * Sets the direction to face
	 */
	void setDirection(int dir);

	/**
	 * Renders the appropriate image/animation
	 *
	 * \param target The target to render to
	 * \param position The on screen position to render to
	 */
	void render(sf::RenderTarget& target, sf::Vector2f position);
};

#endif // ENTITYVISUAL_HPP
