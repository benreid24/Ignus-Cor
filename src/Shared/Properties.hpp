#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

/**
 * This class publicly stores all of the static properties of the game, such as file paths and window resolutions
 *
 * \ingroup Global
 */
class Properties
{
public:
	#ifndef EDITOR
    static const int ScreenWidth = 800;
    static const int ScreenHeight = 600;
    static const int TilesWide = 25;
    static const int TilesTall = 19;
    #else
    static const int ScreenWidth = 1140;
    static const int ScreenHeight = 860;
    static const int TilesWide = 40;
    static const int TilesTall = 27;
    static const std::string EditorResources;
    #endif

    static const std::string GameSavePath;

    static const std::string MapPath;
    static const std::string TilesetFile;
    static const std::string MapTilePath;
    static const std::string MapAnimPath;
    static const std::string SpawnerPath;

    static const std::string EntityImagePath;

    static const std::string AnimationPath;
    static const std::string SpriteSheetPath;

    static const std::string ScriptPath;
    static const std::string ScriptExtension;
    static const std::string FontPath;

    static const std::string PlaylistPath;
    static const std::string MusicPath;
    static const std::string AudioPath;

    static const std::string NpcPath;
    static const std::string ConversationPath;
    static constexpr float ConversationTerminationDistance = 75;

    static const std::string EntityBubbleImageFile;
    static const std::string EntityBubbleImagePath;
    static const std::string EntityBubbleAnimPath;

    static const std::string MenuImagePath;
    static const std::string ItemMenuImagePath;
    static const std::string MiscImagePath;

    static const std::string ItemDbFile;
    static const std::string ItemAnimationPath;

    static const std::string ParticleImagePath;
    static const std::string ParticleAnimPath;

    static sf::Font PrimaryMenuFont;
    static sf::Font SecondaryMenuFont;
    static sf::Font ConversationFont;
    static sf::Font BattleFont;
};

#endif // PROPERTIES_HPP
