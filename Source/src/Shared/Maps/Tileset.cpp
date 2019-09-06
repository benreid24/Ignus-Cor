#include "Shared/Maps/Tileset.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Util/ResourcePool.hpp"
#include "Shared/Util/Util.hpp"
using namespace std;
using namespace sf;

Tileset::Tileset() {
    File loader(Properties::TilesetFile,File::In);
    int n,i;
    string f;

    nextTileId = nextAnimId = 1;
    if (!FileExists(Properties::TilesetFile))
		return;

    for (n = loader.get<uint16_t>(); n>0; --n) {
		i = loader.get<uint16_t>();
		f = loader.getString();
		tileFiles.insert(make_pair(i,f));
		tiles.insert(make_pair(i,imagePool.loadResource(Properties::MapTilePath+f)));
		if (i>=nextTileId)
			nextTileId = i+1;
    }

    for (n = loader.get<uint16_t>(); n>0; --n) {
        i = loader.get<uint16_t>();
        f = loader.getString();
        animFiles.insert(make_pair(i,f));
        anims.insert(make_pair(i,animPool.loadResource(Properties::MapAnimPath+f)));
        if (i>=nextAnimId)
			nextAnimId = i+1;
    }
}

void Tileset::save() {
	File file(Properties::TilesetFile,File::Out);

	file.write<uint16_t>(tileFiles.size());
	for (auto i = tileFiles.begin(); i!=tileFiles.end(); ++i) {
		file.write<uint16_t>(i->first);
		file.writeString(i->second);
	}

	file.write<uint16_t>(animFiles.size());
	for (auto i = animFiles.begin(); i!=animFiles.end(); ++i) {
		file.write<uint16_t>(i->first);
		file.writeString(i->second);
	}
}

int Tileset::addTile(std::string file) {
	string newFile = intToString(nextTileId)+"__"+File::getBaseName(file)+".png";
	copyFile(file,Properties::MapTilePath+newFile);
	tileFiles[nextTileId] = newFile;
	tiles[nextTileId] = imagePool.loadResource(Properties::MapTilePath+newFile);
	nextTileId++;
	return (nextTileId-1);
}

TextureReference Tileset::getTile(int id) {
	return tiles[id];
}

void Tileset::removeTile(int id) {
	tiles.erase(id);
	tileFiles.erase(id);
}

int Tileset::numTiles() {
	return tiles.size();
}

int Tileset::addAnim(std::string file, std::string spritesheet) {
	string newFile = intToString(nextAnimId)+"__"+File::getBaseName(file)+".anim";
	copyFile(file,Properties::MapAnimPath+newFile);
	if (spritesheet!="NONE")
		copyFile(spritesheet,Properties::SpriteSheetPath+File::getBaseName(spritesheet)+".png");
	animFiles[nextAnimId] = newFile;
	anims[nextAnimId] = animPool.loadResource(Properties::MapAnimPath+newFile);
	nextAnimId++;
	return (nextAnimId-1);
}

AnimationReference Tileset::getAnimation(int id) {
	return anims[id];
}

void Tileset::removeAnimation(int id) {
	anims.erase(id);
	animFiles.erase(id);
}

int Tileset::numAnims() {
	return anims.size();
}

vector<int> Tileset::getTileIds() {
	vector<int> ret;
	for (auto i = tiles.begin(); i!=tiles.end(); ++i) {
		ret.push_back(i->first);
	}
	return ret;
}

vector<int> Tileset::getAnimIds() {
	vector<int> ret;
	for (auto i = anims.begin(); i!=anims.end(); ++i) {
		ret.push_back(i->first);
	}
	return ret;
}
