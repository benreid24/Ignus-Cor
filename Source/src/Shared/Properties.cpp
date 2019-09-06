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
const std::string Properties::EntityBubbleImageFile = "Resources/Media/Images/Entities/Bubbles/bubble.png";
const std::string Properties::EntityBubbleImagePath = "Resources/Media/Images/Entities/Bubbles/";
const std::string Properties::EntityBubbleAnimPath = "Resources/Media/Animations/Entities/Bubbles/";
const std::string Properties::MenuImagePath = "Resources/Media/Images/Menu/";
const std::string Properties::MiscImagePath = "Resources/Media/Images/Misc/";
const std::string Properties::ItemDbFile = "Resources/Data/DB/items.db";
const std::string Properties::AttackDbFile = "Resources/Data/DB/attacks.db";
const std::string Properties::EffectDbFile = "Resources/Data/DB/effects.db";
const std::string Properties::ParticleAnimPath = "Resources/Media/Animations/Particles/";
const std::string Properties::ParticleImagePath = "Resources/Media/Images/Particles/";

sf::Font Properties::PrimaryMenuFont;
sf::Font Properties::ConversationFont;

#ifdef EDITOR
const std::string Properties::EditorResources = "Editor/";
#endif // EDITOR
