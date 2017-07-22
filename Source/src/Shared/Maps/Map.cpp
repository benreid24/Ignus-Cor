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

Map::Map(string nm, Vector2i sz, Tileset& tlst, SoundEngine* se, EntityManager* em) : Map(tlst,se) {
	Map::weather->setOwner(this);
	entityManager = em;

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
	entityManager = em;

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
    for (unsigned int i = 0; i<layers.size(); ++i) {
        for (int x = 0; x<size.x; ++x) {
            for (int y = 0; y<size.y; ++y) {
                layers[i](x,y).isAnim = bool(input.get<uint8_t>());
                int id = input.get<uint16_t>();
                layers[i](x,y).id = id;
                layers[i](x,y).nonZero = id!=0;
                layers[i](x,y).anim = nullptr;
                if (layers[i](x,y).isAnim) {
					if (tileset.getAnimation(id)) {
						layers[i](x,y).delA = true;
						if (tileset.getAnimation(id)->isLooping()) { //Animation object is shared between all (sync's water, etc)
							if (animTable.find(id)==animTable.end())
								animTable[id] = new Animation(tileset.getAnimation(id));
							layers[i](x,y).anim = animTable[id];
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
	for (auto i = animTable.begin(); i!=animTable.end(); ++i)
		delete i->second;
	for (unsigned int i = 0; i<layers.size(); ++i) {
		for (int x = 0; x<size.x; ++x) {
			for (int y = 0; y<size.y; ++y) {
				if (layers[i](x,y).delA)
					delete layers[i](x,y).anim;
			}
		}
	}
}

void Map::setScriptEnvironment(ScriptEnvironment* se) {
	scriptEnv = se;
}

string Map::getName() {
	return name;
}

Vector2i Map::getSize() {
	return size;
}

void Map::addVisitedMap(std::string m) {
	if (find(visitedMaps.begin(),visitedMaps.end(),m)==visitedMaps.end())
		visitedMaps.push_back(m);
}

void Map::saveGame(File* saveTo) {
	//TODO - save map data to gamesave
}

void Map::loadGame(File* loadFrom) {
	//TODO - load map data from gamesave
}

void Map::save(std::string file) {
	//TODO - write all data to given file
}

void Map::setWeather(int t) {
	Map::weather->init(Weather::Type(t),true);
}

bool Map::mapVisited(string m) {
	return find(visitedMaps.begin(),visitedMaps.end(),m)!=visitedMaps.end();
}

void Map::update() {
	//TODO - update the map
}

void Map::calculateLighting() {
	if (ambientLightOverride!=255)
        currentLighting = ambientLightOverride;
    else {
		//TODO - make some form of clock
        //ClockTime t = gameClock.getClockTime();
        const int x = 720; //t.hour*60+t.minute;
        const double n = 0.7;
        currentLighting = (70*cos(3.1415926*x/720)+70)*((1-n)*(720-x)*(720-x)/518400+n) + weather->getLightModifier();
        if (currentLighting<0)
            currentLighting = 0;
    }
}

void Map::draw(sf::RenderTarget& target) {
	for (int i = 0; i<firstYSortLayer; ++i) {
        for (int x = camPosTiles.x-10; x<camPosTiles.x+Properties::TilesWide+10; ++x) {
            if (x>=0 && x<size.x)
            for (int y = camPosTiles.y-10; y<camPosTiles.y+Properties::TilesTall+10; ++y) {
                if (y>=0 && y<size.y) {
                    if (layers[i](x,y).isAnim && layers[i](x,y).nonZero) {
                        layers[i](x,y).anim->setPosition(Vector2f(x*32-camPos.x,y*32-camPos.y));
                        layers[i](x,y).anim->draw(target);
                    }
                    else if (layers[i](x,y).nonZero) {
                        layers[i](x,y).spr.setPosition(x*32-camPos.x+32,y*32-camPos.y+32);
                        target.draw(layers[i](x,y).spr);
                    }
                }
            }
        }
    }

    for (int y = camPosTiles.y-10; y<camPosTiles.y+Properties::TilesTall+10; ++y) {
        if (y>=0 && y<size.y) {
            for (int i = 0; i<firstTopLayer-firstYSortLayer; ++i) {
                for (int x = camPosTiles.x-10; x<camPosTiles.x+Properties::TilesWide+10; ++x) {
                    if (x>=0 && x<size.x && ySortedTiles[i](x,y).second) {
                        if (ySortedTiles[i](x,y).second->isAnim && layers[i](x,y).nonZero) {
                            ySortedTiles[i](x,y).second->anim->setPosition(Vector2f(x*32-camPos.x,ySortedTiles[i](x,y).first*32-camPos.y));
                            ySortedTiles[i](x,y).second->anim->draw(target);
                        }
                        else if (ySortedTiles[i](x,y).second->nonZero) {
                            ySortedTiles[i](x,y).second->spr.setPosition(x*32-camPos.x+32,ySortedTiles[i](x,y).first*32-camPos.y+32);
                            target.draw(ySortedTiles[i](x,y).second->spr);
                        }
                    }
                }
            }
            for (unsigned int i = 0; i<entityManager->getYSorted().at(y+1).size(); ++i) {
                entityManager->getYSorted().at(y+1).at(i)->render(target,camPos);
            }
        }
    }

    weather->draw(target);
    if (currentLighting>40) {
        IntRect t(camPos.x-400, camPos.y-300,1600,1200);
        lightTxtr.clear(Color(0,0,0,currentLighting));
        for (unsigned int i = 0; i<lights.size(); ++i) {
            if (t.contains(Vector2i(lights[i].position))) {
                light[0].position = lights[i].position - camPos + Vector2f(32,32);
                light[0].position.y = 600-light[0].position.y;
                light[0].color = Color::Transparent;
                for (unsigned int j = 1; j<362; ++j) {
                    light[j].position = lights[i].position + Vector2f(lights[i].radius*cos(double(j)/180*3.1415926)-camPos.x+32,lights[i].radius*sin(double(j)/180*3.1415926)-camPos.y+32);
                    light[j].color = Color(0,0,0,currentLighting);
                    light[j].position.y = 600-light[j].position.y;
                }
                lightTxtr.draw(light, BlendNone);
            }
        }
        target.draw(lightSpr);
    }

    for (unsigned int i = firstTopLayer; i<layers.size(); ++i) {
        for (int x = camPosTiles.x-10; x<camPosTiles.x+Properties::TilesWide+10; ++x) {
            if (x>=0 && x<size.x)
            for (int y = camPosTiles.y-10; y<camPosTiles.y+Properties::TilesTall+10; ++y) {
                if (y>=0 && y<size.y) {
                    if (layers[i](x,y).isAnim && layers[i](x,y).nonZero) {
                        layers[i](x,y).anim->setPosition(Vector2f(x*32-camPos.x,y*32-camPos.y));
                        layers[i](x,y).anim->draw(target);
                    }
                    else if (layers[i](x,y).nonZero) {
                        layers[i](x,y).spr.setPosition(x*32-camPos.x+32,y*32-camPos.y+32);
                        target.draw(layers[i](x,y).spr);
                    }
                }
            }
        }
    }
}

void Map::setRenderPosition(sf::Vector2f playerPos) {
	camPos = playerPos - Vector2f(Properties::ScreenWidth/2,Properties::ScreenHeight/2);
    if (camPos.x<32)
        camPos.x = 32;
    else if (camPos.x>size.x*32-Properties::ScreenWidth+32)
        camPos.x = size.x*32-Properties::ScreenWidth+32;
    if (camPos.y<32)
        camPos.y = 32;
    else if (camPos.y>size.y*32-Properties::ScreenHeight+32)
        camPos.y = size.y*32-Properties::ScreenHeight+32;

    if (Properties::ScreenWidth>=size.x*32)
        camPos.x = size.x*16-Properties::ScreenWidth/2+32;
    if (Properties::ScreenHeight>=size.y*32)
        camPos.y = size.y*16-Properties::ScreenHeight/2+32;

    camPosTiles.x = camPos.x/32;
    camPosTiles.y = camPos.y/32;
}

Vector2f Map::getCamera() {
	return camPos;
}

void Map::moveOntoTile(sf::Vector2i playerPos, sf::Vector2i lastPos) {
	moveOntoTile(playerPos);
    playerPos.x--;
    playerPos.y--;
    lastPos.x--;
    lastPos.y--;

    for (unsigned int i = 0; i<events.size(); ++i) {
    	int minX = events[i].position.x/32;
    	int minY = events[i].position.y/32;
    	int maxX = minX+events[i].size.x;
    	int maxY = minY+events[i].size.y;
        bool inNow = playerPos.x>=minX && playerPos.x<maxX && playerPos.y>=minY && playerPos.y<maxY;
        bool wasIn = lastPos.x>=minX && lastPos.x<maxX && lastPos.y>=minY && lastPos.y<maxY;
        if ((events[i].trigger==1 && inNow && !wasIn) || (events[i].trigger==2 && !inNow && wasIn) || (events[i].trigger==3 && inNow!=wasIn) || (events[i].trigger==4 && inNow)) {
            if (events[i].runs<events[i].maxRuns || events[i].maxRuns==0)
                Map::scriptEnv->runScript(events[i].script);
            events[i].runs++;
        }
    }
}

void Map::moveOntoTile(sf::Vector2i pos) {
	if (pos.x-1<size.x && pos.y-1<size.y && pos.x-1>=0 && pos.y-1>=0) {
    	for (unsigned int i = 0; i<layers.size(); ++i) {
			if (layers[i](pos.x-1,pos.y-1).isAnim && layers[i](pos.x-1,pos.y-1).nonZero) {
				if (layers[i](pos.x-1,pos.y-1).anim) {
					if (!layers[i](pos.x-1,pos.y-1).anim->isLooping())
						layers[i](pos.x-1,pos.y-1).anim->play();
				}
			}
		}
    }
}

void Map::setCollision(int x, int y, int c) {
	collisions(x,y) = c;
}

int Map::getCollision(int x, int y) {
	return collisions(x,y);
}

bool Map::spaceFree(Vector2i pos, Vector2i oldPos) {
	if (pos.x<=0 || pos.x>size.x || pos.y<=0 || pos.y>size.y)
		return false;

	int dir = 0;
	if (oldPos.x>pos.x)
		dir = 3;
	else if (oldPos.x<pos.x)
		dir = 1;
	else if (oldPos.y<pos.y)
		dir = 2;

    switch (collisions(pos.x,pos.y))
    {
	case 0: //none
		return false;
	case 1: //all
		return true;
	case 2: //top
		return dir==2;
	case 3: //right
		return dir==3;
	case 4: //bottom
		return dir==0;
	case 5: //left
		return dir==1;
	case 6: //topRight
		return dir==2 || dir==3;
	case 7: //bottomRight
		return dir==0 || dir==3;
	case 8: //bottomLeft
		return dir==0 || dir==1;
	case 9: //topLeft
		return dir==1 || dir==2;
	case 10: //topBottom
		return dir==0 || dir==2;
	case 11: //leftRight
		return dir==1 || dir==3;
	case 12: //noTop
		return dir!=2;
	case 13: //noRight
		return dir!=3;
	case 14: //noBottom
		return dir!=0;
	case 15: //noLeft
		return dir!=1;
	default:
		return false;
    }
}

bool Map::spaceFree(Vector2i pos) {
	if (pos.x<=0 || pos.x>size.x || pos.y<=0 || pos.y>size.y)
		return false;

    return collisions(pos.x,pos.y)==1;
}

void Map::setItemPickedUp(int id) {
	pickedUpItems[name].push_back(id);
}

void Map::lockAllPeople() {
	//TODO - lock all people
}

void Map::unlockAllPeople() {
	//TODO - unlock everyone
}

void Map::stopAnimations() {
	//TODO - stop all animations
}

void Map::addLight(int x, int y, int r) {
	Light l;
	l.position.x = x;
	l.position.y = y;
	l.radius = r;
	lights.push_back(l);
}

void Map::removeLight(int x, int y) {
    for (unsigned int i = 0; i<lights.size(); ++i) {
		int d = (lights[i].position.x-x)*(lights[i].position.x-x)+(lights[i].position.y-y)*(lights[i].position.y-y);
		if (d<lights[i].radius*lights[i].radius) {
			lights.erase(lights.begin()+i);
			i--;
		}
    }
}

void Map::removeAllLights() {
	lights.clear();
}

void Map::setLightingOverride(int o) {
	ambientLightOverride = o;
}

void Map::editTile(int x, int y, int layer, int nId, bool isAnim) {
	if (layer>=firstYSortLayer && layer<firstTopLayer) {
		if (layers[layer](x,y).nonZero) {
			int eY = y+layers[layer](x,y).spr.getGlobalBounds().height/64+1;
			if (eY>=size.y)
				eY = size.y-1;
			if (ySortedTiles[layer-firstYSortLayer](x,eY).second) {
				ySortedTiles[layer-firstYSortLayer](x,eY).second->nonZero = false;
			}
		}
	}

	if (layers[layer](x,y).delA) {
		delete layers[layer](x,y).anim;
		layers[layer](x,y).delA = false;
		layers[layer](x,y).anim = nullptr;
	}

	layers[layer](x,y).id = nId;
	layers[layer](x,y).isAnim = isAnim;
	layers[layer](x,y).nonZero = nId!=0;
	if (tileset.getTile(nId) && !isAnim)
		layers[layer](x,y).spr.setTexture(*tileset.getTile(nId));
	else if (tileset.getAnimation(nId) && isAnim) {
		layers[layer](x,y).delA = tileset.getAnimation(nId)->isLooping();
		if (layers[layer](x,y).delA)
			layers[layer](x,y).anim = new Animation(tileset.getAnimation(nId));
		else
			layers[layer](x,y).anim = animTable[nId];
	}

	if (layer>=firstYSortLayer && layer<firstTopLayer) {
		if (layers[layer](x,y).nonZero) {
			int eY = y+layers[layer](x,y).spr.getGlobalBounds().height/64+1; //TODO - possibly handle animations in this range as well
			if (eY>=size.y)
				eY = size.y-1;
			ySortedTiles[layer-firstYSortLayer](x,eY) = make_pair(y,&layers[layer](x,y));
		}
	}
}

void Map::syncAnimTable() {
	vector<int> ids = tileset.getAnimIds();
	vector<int> del;
	for (unsigned int i = 0; i<ids.size(); ++i) {
		if (animTable.find(ids[i])==animTable.end())
			animTable[ids[i]] = new Animation(tileset.getAnimation(ids[i]));
	}
	for (auto i = animTable.begin(); i!=animTable.end(); ++i) {
		if (find(ids.begin(),ids.end(),i->first)==ids.end())
			del.push_back(i->first);
	}
	for (unsigned int i = 0; i<del.size(); ++i) {
		delete animTable[del[i]];
		animTable.erase(del[i]);
	}
}

void Map::clearBrokenTiles() {
	for (unsigned int i = 0; i<layers.size(); +i) {
		for (int x = 0; x<size.x; ++x) {
			for (int y = 0; y<size.y; ++y) {
				if (!tileset.getTile(layers[i](x,y).id) && !layers[i](x,y).isAnim) {
					layers[i](x,y).id = 0;
					layers[i](x,y).nonZero = false;
				}
				else if (!tileset.getAnimation(layers[i](x,y).id) && layers[i](x,y).isAnim) {
					if (layers[i](x,y).delA)
						delete layers[i](x,y).anim;
					layers[i](x,y).id = 0;
					layers[i](x,y).nonZero = false;
					layers[i](x,y).anim = nullptr;
				}
			}
		}
	}
}

void Map::addEvent(MapEvent event) {
	events.push_back(event);
}

void Map::removeEvent(int x, int y) {
	for (unsigned int i = 0; i<events.size(); ++i) {
		if (events[i].position.x<=x && events[i].position.x+events[i].size.x>x && events[i].position.y<=y && events[i].position.y+events[i].size.y>y) {
			events.erase(events.begin()+i);
			i--;
		}
	}
}

MapEvent* Map::getEvent(int x, int y) {
	for (unsigned int i = 0; i<events.size(); ++i) {
		if (events[i].position.x<=x && events[i].position.x+events[i].size.x>x && events[i].position.y<=y && events[i].position.y+events[i].size.y>y)
			return &events[i];
	}
	return nullptr;
}
