#include "Shared/Maps/Map.hpp"
using namespace sf;
using namespace std;

Map::Map(string nm, Vector2i sz, Tileset& tlst, SoundEngine* se) : tileset(tlst), weather(this,se) {
	name = nm;
	size = sz;
	firstYSortLayer = 2;
	firstTopLayer = 5;

	//create tiles
	for (int i = 0; i<5; ++i) {
		layers.push_back(Vector2D<Tile>());
		layers[layers.size()-1].setSize(size.x,size.y);
	}
}
