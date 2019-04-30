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
    const static int ScreenWidth = 800;
    const static int ScreenHeight = 600;
    const static int TilesWide = 25;
    const static int TilesTall = 19;
    #else
    const static int ScreenWidth = 1140;
    const static int ScreenHeight = 860;
    const static int TilesWide = 40;
    const static int TilesTall = 27;
    const static std::string EditorResources;
    #endif

    const static std::string GameSavePath;

    const static std::string MapPath;
    const static std::string TilesetFile;
    const static std::string MapTilePath;
    const static std::string MapAnimPath;
    const static std::string SpawnerPath;

    const static std::string EntityImagePath;

    const static std::string AnimationPath;
    const static std::string SpriteSheetPath;

    const static std::string ScriptPath;
    const static std::string FontPath;

    const static std::string PlaylistPath;
    const static std::string MusicPath;
    const static std::string AudioPath;

    const static std::string NpcPath;
    const static std::string ConversationPath;

    const static std::string MenuImagePath;
    const static std::string MiscImagePath;

    const static std::string ItemDbFile;
    const static std::string WeaponDbFile;
    const static std::string SpellDbFile;

    static sf::Font PrimaryMenuFont;
    static sf::Font SecondaryMenuFont;
    static sf::Font ConversationFont;
    static sf::Font BattleFont;

    static sf::Keyboard::Key upKey;
    static sf::Keyboard::Key rightKey;
    static sf::Keyboard::Key downKey;
    static sf::Keyboard::Key leftKey;
    static sf::Keyboard::Key runKey;
    static sf::Keyboard::Key interactKey;
    static sf::Keyboard::Key pauseKey;
};

#endif // PROPERTIES_HPP
