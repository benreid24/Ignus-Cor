#include "Shared/Maps/Map.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Media/Playlist.hpp"
using namespace sf;
using namespace std;

ScriptEnvironment* Map::scriptEnv = nullptr;
std::vector<std::string> Map::visitedMaps;
std::map<std::string, std::vector<int> > Map::pickedUpItems;
string Map::lastMap, Map::curMap;
Weather* Map::weather = nullptr;
Vector2f Map::lastPos;
sf::RenderTexture Map::lightTxtr;
sf::Sprite Map::lightSpr;
sf::VertexArray Map::light(TrianglesFan, 362);
bool Map::staticMemebersCreated = false;
int Map::lastDir = 2;

Map::Map(Tileset& tlst, SoundEngine* se) : tileset(tlst) {
	if (!Map::staticMemebersCreated) {
		Map::staticMemebersCreated = true;
		Map::lightTxtr.create(Properties::ScreenWidth,Properties::ScreenHeight);
		Map::lightSpr.setTexture(lightTxtr.getTexture());
		Map::weather = new Weather(nullptr,se);
	}
}

Map::Map(string nm, Vector2i sz, Tileset& tlst, SoundEngine* se) : Map(tlst,se) {
	Map::weather->setOwner(this);

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

Map::Map(string file, Tileset& tlst, EntityManager* em, SoundEngine* se, Entity* player, string spName, Playlist* plst) : Map(tlst,se) {
	Map::weather->setOwner(this);

	//Handle previous map data
    if (file=="LastMap")
		file = Map::lastMap;
	Map::lastMap = curMap;
    curMap = file;
    if (spName!="prev") {
        Map::lastPos = player->getPosition();
        Map::lastDir = player->getDir()+2;
        if (Map::lastDir>3)
            Map::lastDir -= 4;
    }

    cout << "Loading map: " << file << "(" << spName << ")\n";

    //Open file
    File input(Properties::MapPath+file+".map");
    int tInt;
    string loadScript;

    //Load name and music
    name = input.getString();
    file = input.getString();
    if (file.size() && plst)
		plst->load(Properties::PlaylistPath+file);
    addVisitedMap(name);

    //Load scripts
    loadScript = input.getString();
    file = input.getString();
	if (File::getExtension(file)=="scr")
		unloadScript.reset(new Script(Properties::ScriptPath+file));
	else
		unloadScript.reset(new Script(file));

	//Load meta data
    size.x = input.get<uint32_t>();
    size.y = input.get<uint32_t>();
    tInt = input.get<uint16_t>();
    layers.resize(tInt);
    collisions.setSize(size.x,size.y);
    for (int i = 0; i<tInt; ++i)
        layers[i].setSize(size.x,size.y);

	//More meta data and weather
    firstYSortLayer = input.get<uint16_t>();
    firstTopLayer = input.get<uint16_t>();
    tInt = input.get<uint8_t>();
    Map::weather->enterMap(name);
    Map::weather->init(Weather::Type(tInt),true);
    tInt = input.get<uint16_t>(); //TODO - pass weather frequency into weather
    ambientLightOverride = input.get<uint16_t>();

    //Allocate memory for y-sorted tiles
    ySortedTiles.resize(firstTopLayer-firstYSortLayer);
    for (unsigned int i = 0; i<ySortedTiles.size(); ++i)
        ySortedTiles[i].setSize(size.x,size.y);

	//Load collisions
    for (int x = 0; x<size.x; ++x)
        for (int y = 0; y<size.y; ++y)
            collisions(x,y) = input.get<uint8_t>();

	//Load tiles
	map<int,Animation*> tempAnimTable;
	anims.reserve(tileset.numAnims()+1);
    for (unsigned int i = 0; i<layers.size(); ++i) {
        for (int x = 0; x<size.x; ++x) {
            for (int y = 0; y<size.y; ++y) {
                layers[i](x,y).isAnim = bool(input.get<uint8_t>());
                int id = input.get<uint16_t>();
                layers[i](x,y).nonZero = id!=0;
                layers[i](x,y).anim = nullptr;
                if (layers[i](x,y).isAnim) {
					if (tileset.getAnimation(id)) {
						layers[i](x,y).delA = true;
						if (tileset.getAnimation(id)->isLooping()) { //Animation object is shared between all (sync's water, etc)
							if (tempAnimTable.find(id)==tempAnimTable.end()) {
								anims.push_back(Animation(tileset.getAnimation(id)));
								tempAnimTable[id] = &anims[anims.size()-1];
							}
							layers[i](x,y).anim = tempAnimTable[id];
							layers[i](x,y).delA = false;
						}
						else //Animation object is unique (so all grass doesn't move at once, etc)
							layers[i](x,y).anim = new Animation(tileset.getAnimation(id));
					}
					else
						layers[i](x,y).nonZero = false;
				}
                else if (id!=0) {
					TextureReference t = tileset.getTile(id);
					if (t)
						layers[i](x,y).spr.setTexture(*tileset.getTile(id));
				}

                if (i>=unsigned(firstYSortLayer) && i<unsigned(firstTopLayer)) {
                    if (layers[i](x,y).nonZero) {
                        int eY = y+layers[i](x,y).spr.getGlobalBounds().height/64+1; //TODO - possibly handle animations in this range as well
                        if (eY>=size.y)
                            eY = size.y-1;
                        ySortedTiles[i-firstYSortLayer](x,eY) = make_pair(y,&layers[i](x,y));
                    }
                }
            }
        }
    }

    //Player spawns
    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i) {
    	string nm;
        Vector2f pos;
        int d;
        nm = input.getString();
        pos.x = input.get<uint32_t>();
        pos.y = input.get<uint32_t>();
        d = input.get<uint8_t>();
        if (spName==nm);
            player->setPositionAndDirection(pos,d);
    }
    if (spName=="prev") {
		player->setPositionAndDirection(Map::lastPos,Map::lastDir);
		player->shift(Vector2f(0,32)); //to move down past the door. if spawning in walls, check this line
	}
    setRenderPosition(player->getPosition());

    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i) {
        Vector2f pos;
        int dir;
        file = input.getString();
        pos.x = input.get<uint32_t>();
        pos.y = input.get<uint32_t>();
        dir = input.get<uint8_t>();
        //TODO - load ai and put into EntityManager
    }

    //Load spawners
    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i) {
		int x = input.get<uint32_t>();
		int y = input.get<uint32_t>();
		int cldwn = input.get<uint16_t>();
		int chance = input.get<uint8_t>();
		int mxAlwd = input.get<uint16_t>();
		string dFile = input.getString();
		//TODO - implement spawners
    }

    //Load items
    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i) {
        int itemId = input.get<uint16_t>();
        int mapId = input.get<uint16_t>();
        Vector2f pos;
        pos.x = input.get<uint32_t>();
        pos.y = input.get<uint32_t>();
        for (unsigned int j = 0; j<pickedUpItems[name].size(); ++j) {
            if (pickedUpItems[name][j]==mapId)
                itemId = -1;
        }
        if (itemId!=-1);
            //TODO - implement items
    }

    //Load lights
    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i) {
        Light temp;
        temp.position.x = input.get<uint32_t>();
        temp.position.y = input.get<uint32_t>();
        temp.radius = input.get<uint16_t>();
        lights.push_back(temp);
    }

    //Load events
    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i)
    {
        MapEvent evt;
        file = input.getString();
        evt.position.x = input.get<uint32_t>();
        evt.position.y = input.get<uint32_t>();
        evt.size.x = input.get<uint16_t>();
        evt.size.y = input.get<uint16_t>();
        evt.maxRuns = input.get<uint8_t>();
        evt.trigger = input.get<uint8_t>();
        evt.runs = 0;
        evt.script.reset(new Script());
        if (File::getExtension(file)=="scr")
            evt.script->load(Properties::ScriptPath+file);
        else
            evt.script->load(file);

        if (evt.trigger==0 && Map::scriptEnv)
            Map::scriptEnv->runScript(evt.script);

        events.push_back(evt);
    }
}

Map::~Map() {
	if (name.size()>0 && unloadScript && Map::scriptEnv)
		unloadScript->run(Map::scriptEnv);
}

void Map::setScriptEnvironment(ScriptEnvironment* se) {
	scriptEnv = se;
}
