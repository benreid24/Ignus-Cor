#include "Shared/Properties.hpp"

const std::string Properties::GameSavePath;
const std::string Properties::MapPath = "Resources/Data/Maps/";
const std::string Properties::TilesetFile = "Resources/Data/tileset.tlst";
const std::string Properties::MapTilePath = "Resources/Media/Map/Tiles/";
const std::string Properties::MapAnimPath = "Resources/Media/Map/Animations/";
const std::string Properties::SpawnerPath = "Resources/Data/Spawners/";
const std::string Properties::EntityImagePath = "Resources/Media/Images/Entities/";
const std::string Properties::AnimationPath = "Resources/Media/Animations/";
const std::string Properties::SpriteSheetPath = "Resources/Media/Spritesheets/";
const std::string Properties::ScriptPath = "Resources/Data/Scripts/";
const std::string Properties::FontPath = "Resources/Media/Fonts/";
const std::string Properties::PlaylistPath = "Resources/Data/Playlists/";
const std::string Properties::MusicPath = "Resources/Media/Music/";
const std::string Properties::AudioPath = "Resources/Media/Audio/";
const std::string Properties::NpcPath;
const std::string Properties::ConversationPath;
const std::string Properties::MenuImagePath = "Resources/Media/Images/Menu/";
const std::string Properties::MiscImagePath = "Resources/Media/Images/Misc/";
const std::string Properties::ItemDbFile = "Resources/Data/Objects/items.db";
bool Properties::debugMode = false;

sf::Font Properties::PrimaryMenuFont;

#ifdef EDITOR
const std::string Properties::EditorResources = "Editor/";
#endif // EDITOR
