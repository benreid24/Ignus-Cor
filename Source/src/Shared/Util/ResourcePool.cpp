#include "Shared/Util/ResourcePool.hpp"
#include "Shared/Media/Animation.hpp"

ResourcePool<sf::Texture> imagePool;
ResourcePool<AnimationSource> animPool;
ResourcePool<Script> scriptPool;
ResourcePool<sf::SoundBuffer> audioPool;

template<>
sf::SoundBuffer* loadResourceFromUri(std::string file)
{
    sf::SoundBuffer* temp = new sf::SoundBuffer();
    temp->loadFromFile(file);
    return temp;
}

template<>
sf::Texture* loadResourceFromUri(std::string file)
{
    sf::Texture* temp = new sf::Texture();
    temp->loadFromFile(file);
    return temp;
}

template<>
AnimationSource* loadResourceFromUri(std::string file)
{
    return new AnimationSource(file);
}
