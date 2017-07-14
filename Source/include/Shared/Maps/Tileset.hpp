#ifndef TILESET_HPP
#define TILESET_HPP

#include "Shared/Util/ResourceTypes.hpp"
#include <map>
#include <string>

/**
 * Storage class that manages the tiles and animations for the map
 */
class Tileset {
	std::map<int,TextureReference> tiles;
	std::map<int,AnimationReference> anims;
	std::map<int,std::string> tileFiles, animFiles;
	int nextTileId, nextAnimId;

public:
	/**
	 * Loads the tileset from the tileset file. Note that one global tileset is being used for the entire game
	 */
	Tileset();

	/**
	 * Saves the tileset
	 */
	void save();

	/**
	 * Adds a tile to the tileset. This function will copy the given file into the game directory and give it a new name
	 *
	 * \param file The file of the tile to add
	 * \return The id of the new tile
	 */
	int addTile(std::string file);

	/**
	 * Returns a reference to the requested tile
	 *
	 * \param id The id of the tile to fetch
	 * \return A reference to the requested tile
	 */
	TextureReference getTile(int id);

	/**
	 * Deletes the given tile from the tileset
	 *
	 * \param id The id of the tile to delete
	 */
	void removeTile(int id);

	/**
	 * Adds the given animation to the tileset, copying it into the game directory as well
	 *
	 * \param file The file pointing to the animation itself
	 * \param spritesheet The spritesheet to move over as well. This may be omitted if the spritesheet is already in place
	 * \return The id of the new animation
	 */
	int addAnim(std::string file, std::string spritesheet = "NONE");

	/**
	 * Returns the requested animation from the tileset
	 *
	 * \param id The id of the animation to fetch
	 * \return A reference to the animation requested
	 */
	AnimationReference getAnimation(int id);

	/**
	 * Deletes the specified animation from the tileset
	 *
	 * \param id The id of the animation to delete
	 */
	void removeAnimation(int id);

	/**
	 * Returns a vector containing all of the valid tile ids
	 */
	std::vector<int> getTileIds();

	/**
	 * Returns a vector containing all of the valid animation ids
	 */
	std::vector<int> getAnimIds();
};

#endif // TILESET_HPP
