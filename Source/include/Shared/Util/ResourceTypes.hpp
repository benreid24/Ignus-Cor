#ifndef RESOURCETYPES_HPP
#define RESOURCETYPES_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Shared/Media/Animation.hpp"

typedef std::shared_ptr<sf::Texture> TextureReference;
typedef std::shared_ptr<sf::SoundBuffer> SoundReference;
typedef std::shared_ptr<AnimationSource> AnimationReference;

#endif // RESOURCETYPES_HPP
