#include "Shared/Maps/Map.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Media/Playlist.hpp"
#include "Shared/Data/ItemDB.hpp"
#include "Shared/Scripts/ScriptManager.hpp"
#include <cmath>
using namespace sf;
using namespace std;

#ifdef EDITOR
namespace {
	const string collisionFiles[16] = {Properties::EditorResources+"none.png",
									   Properties::EditorResources+"all.png",
									   Properties::EditorResources+"top.png",
  									   Properties::EditorResources+"right.png",
	  								   Properties::EditorResources+"bottom.png",
		  							   Properties::EditorResources+"left.png",
									   Properties::EditorResources+"topRight.png",
									   Properties::EditorResources+"bottomRight.png",
									   Properties::EditorResources+"bottomLeft.png",
									   Properties::EditorResources+"topLeft.png",
									   Properties::EditorResources+"topBottom.png",
									   Properties::EditorResources+"leftRight.png",
									   Properties::EditorResources+"noTop.png",
									   Properties::EditorResources+"noRight.png",
									   Properties::EditorResources+"noBottom.png",
									   Properties::EditorResources+"noLeft.png"};
}
#endif

std::vector<std::string> Map::visitedMaps;
std::map<std::string, std::vector<int> > Map::pickedUpItems;
Weather* Map::weather = nullptr;
sf::RenderTexture Map::lightTxtr;
sf::Sprite Map::lightSpr;
sf::VertexArray Map::light(TrianglesFan, 362);
bool Map::staticMemebersCreated = false;
vector<TextureReference> Map::collisionTextures;
Sprite Map::collisionSprite;

Map::Map(Tileset& tlst) : tileset(tlst) {
	maxMapItemId = 1;

	#ifdef EDITOR
	arrowTexture = imagePool.loadResource(Properties::EditorResources+"arrow.png");
	arrowSprite.setTexture(*arrowTexture);
	arrowSprite.setOrigin(arrowTexture->getSize().x/2,arrowTexture->getSize().y/2);
	utilText.setFillColor(Color::Black);
	utilText.setCharacterSize(13);
	utilText.setFont(Properties::PrimaryMenuFont);
	spawnerTexture = imagePool.loadResource(Properties::EditorResources+"spawner.png");
	spawnerSprite.setTexture(*spawnerTexture);
	#endif

	if (!Map::staticMemebersCreated) {
		Map::staticMemebersCreated = true;
		Map::lightTxtr.create(Properties::ScreenWidth,Properties::ScreenHeight);
		Map::lightSpr.setTexture(lightTxtr.getTexture());
		Map::weather = new Weather(this);
		#ifdef EDITOR
		for (int i = 0; i<16; ++i) {
            Map::collisionTextures.push_back(imagePool.loadResource(collisionFiles[i]));
		}
		#endif
	}
}

Map::Map(string nm, string uniqueNm, Vector2i sz, Tileset& tlst, int nLayers, int firstYSort, int firstTop) : Map(tlst) {
	Map::weather->setOwner(this);

	name = nm;
	uniqueName = uniqueNm;
	size = sz;
	firstYSortLayer = firstYSort;
	firstTopLayer = firstTop;

	ambientLightOverride = 0;
	weatherFreq = 0;
	weatherType = 0;

	//create tiles
	for (int i = 0; i<nLayers; ++i) {
		layers.push_back(Vector2D<Tile>());
		layers[layers.size()-1].setSize(size.x,size.y);
	}
	collisions.setSize(size.x,size.y);
	resetYSorted();
	EntityManager::get()->registerMap(uniqueNm, size.y);
}

Map::Map(string file, Tileset& tlst, Entity::Ptr player) : Map(tlst) {
	Map::weather->setOwner(this);
	uniqueName = file;

    cout << "Loading map: " << file << "\n";

    //Open file
    File input(Properties::MapPath+file);
    int tInt;

    //Load name and music
    name = input.getString();
    string temp = input.getString();
    //if (temp.size())
	//	Playlist::get()->load(Properties::PlaylistPath+temp); //TODO - music manager
	music = temp;
    addVisitedMap(name);

    //Load scripts
    loadScriptStr = input.getString();
    unloadScriptStr = input.getString();
	if (File::getExtension(unloadScriptStr)=="scr")
		unloadScript.reset(new Script(Properties::ScriptPath+unloadScriptStr));
	else
		unloadScript.reset(new Script(unloadScriptStr));

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
    weatherType = input.get<uint8_t>();
    Map::weather->enterMap(name);
    Map::weather->init(Weather::Type(weatherType),true);
    weatherFreq = input.get<uint16_t>(); //TODO - pass weather frequency into weather
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

    //Load player spawns
    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i) {
        EntitySpawn tmp;
        tmp.name = input.getString();
        tmp.position.coords.x = input.get<uint32_t>();
        tmp.position.coords.y = input.get<uint32_t>();
        tmp.position.dir = EntityPosition::Direction(input.get<uint8_t>());
        tmp.position.mapName = uniqueName;
		entitySpawns.push_back(tmp);
    }
	if (player!=nullptr)
		setRenderPosition(player->getPosition().coords);

    //Load AI
	EntityManager::get()->registerMap(uniqueName, size.y);
    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i) {
        Vector2f pos;
        file = input.getString();
        pos.x = input.get<uint32_t>();
        pos.y = input.get<uint32_t>();
        int dir = input.get<uint8_t>();
		//TODO - load ai and put into EntityManager
    }

    //Load spawners
    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i) {
		MapSpawner sp;
		sp.position.x = input.get<uint32_t>();
		sp.position.y = input.get<uint32_t>();
		sp.coolDown = input.get<uint16_t>();
		sp.frequency = input.get<uint8_t>();
		sp.spawnLimit = input.get<uint16_t>();
		sp.numActiveLimit = input.get<uint16_t>();
		sp.templateFile = input.getString();
		spawners.push_back(sp);
    }

    //Load items
    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i) {
		MapItem item;
        item.itemId = input.get<uint16_t>();
        item.mapId = input.get<uint16_t>();
        item.position.x = input.get<uint32_t>();
        item.position.y = input.get<uint32_t>();
        if (!ItemDB::itemExists(item.itemId)) {
			cout << "Error: Item [" << item.itemId << "] does not exist, removing from map" << endl;
			continue;
        }
        for (unsigned int j = 0; j<pickedUpItems[uniqueName].size(); ++j) {
            if (pickedUpItems[uniqueName][j]==item.mapId)
                item.itemId = -1;
        }
        if (item.itemId!=-1) {
			EntityPosition pos;
			pos.coords = Vector2f(item.position);
			pos.mapName = uniqueName;
			item.ie = ItemEntity::create(item.itemId,pos);
            items.push_back(item);
            EntityManager::get()->add(item.ie);
        }
		if (item.mapId>maxMapItemId)
			maxMapItemId = item.mapId+1;
    }

    //Load lights
    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i) {
        Light temp;
        temp.position.x = input.get<uint32_t>();
        temp.position.y = input.get<uint32_t>();
        temp.radius = input.get<uint16_t>();
        temp.threshold = input.get<uint8_t>();
        lights.push_back(temp);
    }

    //Load events
    tInt = input.get<uint16_t>();
    for (int i = 0; i<tInt; ++i) {
        MapEvent evt;
        evt.scriptStr = input.getString();
        evt.position.x = input.get<uint32_t>();
        evt.position.y = input.get<uint32_t>();
        evt.size.x = input.get<uint16_t>();
        evt.size.y = input.get<uint16_t>();
        evt.maxRuns = input.get<uint8_t>();
        evt.trigger = input.get<uint8_t>();
        evt.runs = 0;
        evt.script.reset(new Script());
        if (File::getExtension(evt.scriptStr)=="scr")
            evt.script->load(Properties::ScriptPath+evt.scriptStr);
        else
            evt.script->load(evt.scriptStr);

        if (evt.trigger==0)
            ScriptManager::get()->runScript(makeMapScript(evt.script, evt, nullptr));

        events.push_back(evt);
    }

    //Run on-load script
    Script loadScript(loadScriptStr);
    loadScript.run();

    //Clean up Editor only data
    #ifndef EDITOR
    items.clear();
    #endif // EDITOR
}

Map::~Map() {
	if (name.size()>0 && unloadScript)
		unloadScript->run();
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

void Map::spawnEntity(Entity::Ptr e, string spawn) {
    EntitySpawn spawnPoint;
    //TODO - what to do if spawn is filled?
    for (unsigned int i = 0; i<entitySpawns.size(); ++i) {
        if (entitySpawns[i].name==spawn)
            spawnPoint = entitySpawns[i];
    }
    e->setPositionAndDirection(spawnPoint.position);

	if (e->getType()=="Player")
		setRenderPosition(e->getPosition().coords);

}

void Map::resetYSorted() {
	ySortedTiles.clear();
	ySortedTiles.resize(firstTopLayer-firstYSortLayer);
    for (unsigned int i = 0; i<ySortedTiles.size(); ++i) {
        ySortedTiles[i].setSize(size.x,size.y);
    }

	for (int i = firstYSortLayer; i<firstTopLayer; ++i) {
		for (int x = 0; x<size.x; ++x) {
			for (int y = 0; y<size.y; ++y) {
				if (layers[i](x,y).nonZero) {
					int eY = y+layers[i](x,y).spr.getGlobalBounds().height/64+1;
					if (eY>=size.y)
						eY = size.y-1;
					ySortedTiles[i-firstYSortLayer](x,eY) = make_pair(y,&layers[i](x,y));
				}
			}
		}
	}
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
	//Open file
    File output(file,File::Out);

    //Write name and music
    output.writeString(name);
    output.writeString(music);

    //Write scripts
    output.writeString(loadScriptStr);
    output.writeString(unloadScriptStr);

	//Save meta data
    output.write<uint32_t>(size.x);
    output.write<uint32_t>(size.y);
    output.write<uint16_t>(layers.size());

	//Save meta data and weather
    output.write<uint16_t>(firstYSortLayer);
    output.write<uint16_t>(firstTopLayer);
    output.write<uint8_t>(weatherType);
    output.write<uint16_t>(weatherFreq);
    output.write<uint16_t>(ambientLightOverride);

	//Save collisions
    for (int x = 0; x<size.x; ++x) {
        for (int y = 0; y<size.y; ++y) {
            output.write<uint8_t>(collisions(x,y));
        }
    }

	//Save tiles
    for (unsigned int i = 0; i<layers.size(); ++i) {
        for (int x = 0; x<size.x; ++x) {
            for (int y = 0; y<size.y; ++y) {
				output.write<uint8_t>(int(layers[i](x,y).isAnim));
                output.write<uint16_t>(layers[i](x,y).id);
            }
        }
    }

    //Save player spawns
    output.write<uint16_t>(entitySpawns.size());
    for (unsigned int i = 0; i<entitySpawns.size(); ++i) {
        output.writeString(entitySpawns[i].name);
        output.write<uint32_t>(entitySpawns[i].position.coords.x);
        output.write<uint32_t>(entitySpawns[i].position.coords.y);
        output.write<uint8_t>(entitySpawns[i].position.dir);
    }

    //Save AI
    output.write<uint16_t>(0);
    /*for (int i = 0; i<tInt; ++i) { //TODO - save ai according to below format
        Vector2f pos;
        int dir;
        file = output.writeString();
        pos.x = output.write<uint32_t>();
        pos.y = output.write<uint32_t>();
        dir = output.write<uint8_t>();
        //TODO - load ai and put into EntityManager
    }*/

    //Save spawners
    output.write<uint16_t>(spawners.size());
    for (unsigned int i = 0; i<spawners.size(); ++i) {
		output.write<uint32_t>(spawners[i].position.x);
		output.write<uint32_t>(spawners[i].position.y);
		output.write<uint16_t>(spawners[i].coolDown);
		output.write<uint8_t>(spawners[i].frequency);
		output.write<uint16_t>(spawners[i].spawnLimit);
		output.write<uint16_t>(spawners[i].numActiveLimit);
		output.writeString(spawners[i].templateFile);
    }

    //Save items
    output.write<uint16_t>(items.size());
    for (unsigned int i = 0; i<items.size(); ++i) {
        output.write<uint16_t>(items[i].itemId);
        output.write<uint16_t>(items[i].mapId);
        output.write<uint32_t>(items[i].position.x);
        output.write<uint32_t>(items[i].position.y);
    }

    //Save lights
    output.write<uint16_t>(lights.size());
    for (unsigned int i = 0; i<lights.size(); ++i) {
        output.write<uint32_t>(lights[i].position.x);
        output.write<uint32_t>(lights[i].position.y);
        output.write<uint16_t>(lights[i].radius);
        output.write<uint8_t>(lights[i].threshold);
    }

    //Save events
    output.write<uint16_t>(events.size());
    for (unsigned int i = 0; i<events.size(); ++i)
    {
        output.writeString(events[i].scriptStr);
        output.write<uint32_t>(unsigned(events[i].position.x));
        output.write<uint32_t>(unsigned(events[i].position.y));
        output.write<uint16_t>(events[i].size.x);
        output.write<uint16_t>(events[i].size.y);
        output.write<uint8_t>(events[i].maxRuns);
        output.write<uint8_t>(events[i].trigger);
    }
}

void Map::setWeather(int t) {
	weatherType = t;
	Map::weather->init(Weather::Type(t),true);
}

int Map::getWeatherType() {
	return weatherType;
}

int& Map::getWeatherFrequency() {
	return weatherFreq;
}

string& Map::getLoadScript() {
	return loadScriptStr;
}

string& Map::getUnloadScript() {
	return unloadScriptStr;
}

bool Map::mapVisited(string m) {
	return find(visitedMaps.begin(),visitedMaps.end(),m)!=visitedMaps.end();
}

void Map::update() {
	//TODO - update the map
	Map::weather->update();
	calculateLighting();
	for (map<int,Animation*>::iterator i = animTable.begin(); i!=animTable.end(); ++i) {
		i->second->update();
	}
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
                        layers[i](x,y).spr.setPosition(x*32-camPos.x,y*32-camPos.y);
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
                        if (ySortedTiles[i](x,y).second->isAnim && ySortedTiles[i](x,y).second->nonZero) {
                            ySortedTiles[i](x,y).second->anim->setPosition(Vector2f(x*32-camPos.x,ySortedTiles[i](x,y).first*32-camPos.y));
                            ySortedTiles[i](x,y).second->anim->draw(target);
                        }
                        else if (ySortedTiles[i](x,y).second->nonZero) {
                            ySortedTiles[i](x,y).second->spr.setPosition(x*32-camPos.x,ySortedTiles[i](x,y).first*32-camPos.y);
                            target.draw(ySortedTiles[i](x,y).second->spr);
                        }
                    }
                }
            }
            for (unsigned int i = 0; i<EntityManager::get()->getYSorted(uniqueName).at(y).size(); ++i) {
                EntityManager::get()->getYSorted(uniqueName).at(y).at(i)->render(target,camPos);
            }
        }
    }

    weather->draw(target);
	IntRect t(camPos.x-Properties::ScreenWidth/2, camPos.y-Properties::ScreenHeight/2,Properties::ScreenWidth*2,Properties::ScreenHeight*2);
	lightTxtr.clear(Color(0,0,0,currentLighting));
	for (unsigned int i = 0; i<lights.size(); ++i) {
		if (t.contains(Vector2i(lights[i].position)) && currentLighting>=lights[i].threshold) {
			light[0].position = lights[i].position - camPos + Vector2f(32,32);
			light[0].position.y = Properties::ScreenHeight-light[0].position.y;
			light[0].color = Color::Transparent;
			for (unsigned int j = 1; j<362; ++j) {
				light[j].position = lights[i].position + Vector2f(lights[i].radius*cos(double(j)/180*3.1415926)-camPos.x,lights[i].radius*sin(double(j)/180*3.1415926)-camPos.y);
				light[j].color = Color(0,0,0,currentLighting);
				light[j].position.y = Properties::ScreenHeight-light[j].position.y;
			}
			lightTxtr.draw(light, BlendNone);
		}
	}
	target.draw(lightSpr);

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
                        layers[i](x,y).spr.setPosition(x*32-camPos.x,y*32-camPos.y);
                        target.draw(layers[i](x,y).spr);
                    }
                }
            }
        }
    }
}

void Map::draw(sf::RenderTarget& target, vector<int> filter, IntRect selection, bool showEvents) {
	for (int i = 0; i<firstYSortLayer; ++i) {
		if (find(filter.begin(),filter.end(),i)==filter.end())
			continue;
        for (int x = camPosTiles.x-10; x<camPosTiles.x+Properties::TilesWide+10; ++x) {
            if (x>=0 && x<size.x)
            for (int y = camPosTiles.y-10; y<camPosTiles.y+Properties::TilesTall+10; ++y) {
                if (y>=0 && y<size.y) {
                    if (layers[i](x,y).isAnim && layers[i](x,y).nonZero) {
                        layers[i](x,y).anim->setPosition(Vector2f(x*32-camPos.x,y*32-camPos.y));
                        layers[i](x,y).anim->draw(target);
                    }
                    else if (layers[i](x,y).nonZero) {
                        layers[i](x,y).spr.setPosition(x*32-camPos.x,y*32-camPos.y);
                        target.draw(layers[i](x,y).spr);
                    }
                }
            }
        }
    }

    for (int y = camPosTiles.y-10; y<camPosTiles.y+Properties::TilesTall+10; ++y) {
        if (y>=0 && y<size.y) {
            for (int i = 0; i<firstTopLayer-firstYSortLayer; ++i) {
				if (find(filter.begin(),filter.end(),i+firstYSortLayer)==filter.end())
					continue;
                for (int x = camPosTiles.x-10; x<camPosTiles.x+Properties::TilesWide+10; ++x) {
                    if (x>=0 && x<size.x && ySortedTiles[i](x,y).second) {
                        if (ySortedTiles[i](x,y).second->isAnim && ySortedTiles[i](x,y).second->nonZero) {
                            ySortedTiles[i](x,y).second->anim->setPosition(Vector2f(x*32-camPos.x,ySortedTiles[i](x,y).first*32-camPos.y));
                            ySortedTiles[i](x,y).second->anim->draw(target);
                        }
                        else if (ySortedTiles[i](x,y).second->nonZero) {
                            ySortedTiles[i](x,y).second->spr.setPosition(x*32-camPos.x,ySortedTiles[i](x,y).first*32-camPos.y);
                            target.draw(ySortedTiles[i](x,y).second->spr);
                        }
                    }
                }
            }
            for (unsigned int i = 0; i<EntityManager::get()->getYSorted(uniqueName).at(y).size(); ++i) {
                EntityManager::get()->getYSorted(uniqueName).at(y).at(i)->render(target,camPos);
            }
        }
    }

    weather->draw(target);
	IntRect t(camPos.x-Properties::ScreenWidth/2, camPos.y-Properties::ScreenHeight/2,Properties::ScreenWidth*2,Properties::ScreenHeight*2);
	lightTxtr.clear(Color(0,0,0,currentLighting));
	for (unsigned int i = 0; i<lights.size(); ++i) {
		if (t.contains(Vector2i(lights[i].position)) && currentLighting>=lights[i].threshold) {
			light[0].position = lights[i].position - camPos + Vector2f(32,32);
			light[0].position.y = Properties::ScreenHeight-light[0].position.y;
			light[0].color = Color::Transparent;
			for (unsigned int j = 1; j<362; ++j) {
				light[j].position = lights[i].position + Vector2f(lights[i].radius*cos(double(j)/180*3.1415926)-camPos.x,lights[i].radius*sin(double(j)/180*3.1415926)-camPos.y);
				light[j].color = Color(0,0,0,currentLighting);
				light[j].position.y = Properties::ScreenHeight-light[j].position.y;
			}
			lightTxtr.draw(light, BlendNone);
		}
	}
	target.draw(lightSpr);

    for (unsigned int i = firstTopLayer; i<layers.size(); ++i) {
		if (find(filter.begin(),filter.end(),i)==filter.end())
			continue;
        for (int x = camPosTiles.x-10; x<camPosTiles.x+Properties::TilesWide+10; ++x) {
            if (x>=0 && x<size.x)
            for (int y = camPosTiles.y-10; y<camPosTiles.y+Properties::TilesTall+10; ++y) {
                if (y>=0 && y<size.y) {
                    if (layers[i](x,y).isAnim && layers[i](x,y).nonZero) {
                        layers[i](x,y).anim->setPosition(Vector2f(x*32-camPos.x,y*32-camPos.y));
                        layers[i](x,y).anim->draw(target);
                    }
                    else if (layers[i](x,y).nonZero) {
                        layers[i](x,y).spr.setPosition(x*32-camPos.x,y*32-camPos.y);
                        target.draw(layers[i](x,y).spr);
                    }
                }
            }
        }
    }

    if (find(filter.begin(),filter.end(),-1)!=filter.end()) {
		for (int x = camPosTiles.x-10; x<camPosTiles.x+Properties::TilesWide+10; ++x) {
			if (x>=0 && x<size.x)
			for (int y = camPosTiles.y-10; y<camPosTiles.y+Properties::TilesTall+10; ++y) {
				if (y>=0 && y<size.y) {
					Map::collisionSprite.setTexture(*Map::collisionTextures[collisions(x,y)]);
					Map::collisionSprite.setPosition(x*32-camPos.x,y*32-camPos.y);
					target.draw(Map::collisionSprite);
				}
			}
		}
    }

    if (selection.left>=0) {
		RectangleShape rect(Vector2f(selection.width*32,selection.height*32));
		rect.setFillColor(Color(170,170,20,70));
		rect.setPosition(selection.left*32-camPos.x,selection.top*32-camPos.y);
		target.draw(rect);
    }

    if (showEvents) {
		RectangleShape rect(Vector2f(32,32));
		rect.setFillColor(Color(0,0,0,130));

		for (unsigned int i = 0; i<events.size(); ++i) {
			rect.setPosition(events[i].position.x*32-camPos.x, events[i].position.y*32-camPos.y);
			rect.setSize(Vector2f(events[i].size.x*32,events[i].size.y*32));
			target.draw(rect);
		}
    }

    for (unsigned int i = 0; i<entitySpawns.size(); ++i) {
        arrowSprite.setPosition(entitySpawns[i].position.coords.x*32-camPos.x+arrowTexture->getSize().x/2,entitySpawns[i].position.coords.y*32-camPos.y+arrowTexture->getSize().y/2);
        arrowSprite.setRotation(entitySpawns[i].position.dir*90);
        target.draw(arrowSprite);
        utilText.setString(entitySpawns[i].name);
        utilText.setPosition(entitySpawns[i].position.coords.x*32-camPos.x+16-utilText.getCharacterSize()*entitySpawns[i].name.size()/4,entitySpawns[i].position.coords.y*32-camPos.y+10);
        target.draw(utilText);
    }

    for (unsigned int i = 0; i<spawners.size(); ++i) {
		spawnerSprite.setPosition(spawners[i].position.x*32-camPos.x,spawners[i].position.y*32-camPos.y);
		utilText.setString(spawners[i].templateFile);
		utilText.setPosition(spawners[i].position.x*32-camPos.x+16-utilText.getCharacterSize()*spawners[i].templateFile.size()/4,spawners[i].position.y*32-camPos.y+10);
        target.draw(spawnerSprite);
        target.draw(utilText);
    }
}

void Map::setRenderPosition(sf::Vector2f playerPos) {
	FloatRect bounds(0,0,size.x*32-Properties::ScreenWidth,size.y*32-Properties::ScreenHeight);
	#ifdef EDITOR
	bounds.left = bounds.top = -32;
	bounds.width = size.x*32-Properties::ScreenWidth+32;
	bounds.height = size.y*32-Properties::ScreenHeight+32;
	#endif // EDITOR

	camPos = playerPos - Vector2f(Properties::ScreenWidth/2,Properties::ScreenHeight/2);
    if (camPos.x<bounds.left)
        camPos.x = bounds.left;
    else if (camPos.x>bounds.width)
        camPos.x = bounds.width;
    if (camPos.y<bounds.top)
        camPos.y = bounds.top;
    else if (camPos.y>bounds.height)
        camPos.y = bounds.height;

    if (Properties::ScreenWidth>=size.x*32)
        camPos.x = size.x*16-Properties::ScreenWidth/2;
    if (Properties::ScreenHeight>=size.y*32)
        camPos.y = size.y*16-Properties::ScreenHeight/2;

    camPosTiles.x = camPos.x/32;
    camPosTiles.y = camPos.y/32;
}

void Map::resize(Vector2i sz, bool useTop, bool useLeft) {
	//Determine mapping from old tiles to new tiles
	int dx = size.x-sz.x;
	int dy = size.y-sz.y;
	int xOffset = useLeft?(-dx):(0);
	int yOffset = useTop?(-dy):(0);

	//Allocate new map
	vector<Vector2D<Tile> > tiles;
	Vector2D<Tile> layer(sz.x,sz.y);
	for (unsigned int i = 0; i<layers.size(); ++i)
		tiles.push_back(layer);

	//Swap new tiles for old
	swap(layers,tiles);

	//Initialize new tiles to all empty
	for (unsigned int i = 0; i<layers.size(); ++i) {
		for (int x = 0; x<sz.x; ++x) {
			for (int y = 0; y<sz.y; ++y) {
                editTile(x, y, i, 0, false);
			}
		}
	}

	//Copy over tiles based on crop pattern
	for (int x = 0; x<size.x; ++x) {
		for (int y = 0; y<size.y; ++y) {
			for (unsigned int i = 0; i<layers.size(); ++i) {
				if (x+xOffset<sz.x && y+yOffset<sz.y && x+xOffset>=0 && y+yOffset>=0)
					editTile(x+xOffset,y+yOffset,i,tiles[i](x,y).id,tiles[i](x,y).isAnim);
			}
		}
	}

	//Update size and reset y-sort
	size = sz;
	resetYSorted();
}

void Map::addLayer(int i) {
	Vector2D<Tile> layer;
	layer.setSize(size.x,size.y);
	if (i>=0 && unsigned(i)<layers.size())
        layers.insert(layers.begin()+i,layer);
	else
		layers.push_back(layer);
}

void Map::removeLayer(int i) {
	if (i>=0 && unsigned(i)<layers.size()) {
		layers.erase(layers.begin()+i);
		if (unsigned(firstTopLayer)>=layers.size())
			firstTopLayer = layers.size()-1;
		if (firstTopLayer<0)
			firstTopLayer = 0;
		if (unsigned(firstYSortLayer)>=layers.size())
			firstYSortLayer = layers.size()-1;
		if (firstYSortLayer<0)
			firstYSortLayer = 0;
		resetYSorted();
	}
}

int Map::getLayerCount() {
	return layers.size();
}

void Map::setFirstYSortLayer(int i) {
	if (i>=0 && unsigned(i)<layers.size())
		firstYSortLayer = i;
}

int Map::getFirstYSortLayer() {
	return firstYSortLayer;
}

void Map::setFirstTopLayer(int i) {
	if (i>=0 && unsigned(i)<layers.size())
		firstTopLayer = i;
}

int Map::getFirstTopLayer() {
	return firstTopLayer;
}

string& Map::getMusic() {
	return music;
}

Vector2f Map::getCamera() {
	return camPos;
}

void Map::moveOntoTile(Entity::Ptr ent, sf::FloatRect oldBox) {
    FloatRect box = ent->getBoundingBox();
    int minX = round(box.left/32+0.5);
    int minY = round(box.top/32+0.5);
    int maxX = ceil((box.left+box.width)/32-0.5);
    int maxY = ceil((box.top+box.height)/32-0.5);

    //TODO - refine on tile detection

    for (unsigned int i = 0; i<layers.size(); ++i) {
        for (int x = minX; x<=maxX; ++x) {
            for (int y = 0; y<=maxY; ++y) {
                if (x>=0 && y>=0 && x<size.x && y<size.y) {
                    if (layers[i](x,y).isAnim && layers[i](x,y).nonZero) {
                        if (layers[i](x,y).anim) {
                            if (!layers[i](x,y).anim->isLooping() && !layers[i](x,y).anim->isPlaying())
                                layers[i](x,y).anim->play();
                        }
                    }
                }
            }
        }
    }



    for (unsigned int i = 0; i<events.size(); ++i) {
        FloatRect eventBox(events[i].position.x*32, events[i].position.y*32, events[i].size.x*32, events[i].size.y*32);
        bool inNow = box.intersects(eventBox);
        bool wasIn = oldBox.intersects(eventBox);
        if ((events[i].trigger==1 && inNow && !wasIn) || (events[i].trigger==2 && !inNow && wasIn) || (events[i].trigger==3 && inNow!=wasIn) || (events[i].trigger==4 && inNow)) {
            if (events[i].runs<events[i].maxRuns || events[i].maxRuns==0)
                ScriptManager::get()->runScript(makeMapScript(events[i].script, events[i], ent));
            events[i].runs++;
        }
    }
}

void Map::setCollision(int x, int y, int c) {
	collisions(x,y) = c;
}

int Map::getCollision(int x, int y) {
	return collisions(x,y);
}

vector<Vector2i> Map::getCoveredTiles(FloatRect box) {
	vector<Vector2i> ret;
	int bx = box.left/32;
	int by = box.top/32;
	int w = (box.width+15)/32;
	int h = (box.height+5)/32;
	if (box.width-w*32>0)
		w++;
	if (box.height-h*32>0)
		h++;
	int tx = (box.left+box.width)/32;
	int ty = (box.top+box.height)/32;
	for (int x = bx; x<=tx; ++x) {
		for (int y = by; y<=ty; ++y)
			ret.push_back(Vector2i(x,y));
	}
	return ret;
}

bool Map::spaceFree(FloatRect oldBox, FloatRect newBox) {
	if (newBox.left<0 || newBox.top<0 || newBox.left+newBox.width>=size.x*32 || newBox.top+newBox.height>=size.y*32)
		return false;

	int dir = 0;
	if (newBox.left>oldBox.left)
		dir = 1;
	else if (newBox.left<oldBox.left)
		dir = 3;
	else if (newBox.top>oldBox.top)
		dir = 2;

	vector<Vector2i> tiles = Map::getCoveredTiles(newBox);
	vector<Vector2i> oldTiles = Map::getCoveredTiles(oldBox);

	for (unsigned int i = 0; i<tiles.size(); ++i) {
		if (find(oldTiles.begin(), oldTiles.end(), tiles[i])!=oldTiles.end())
			continue;

		if (tiles[i].x<0 || tiles[i].x>=size.x || tiles[i].y<0 || tiles[i].y>=size.y)
			return false;

		switch (collisions(tiles[i].x,tiles[i].y))
		{
		case 0: //none
			return false;

		case 1: //all
			continue;

		case 2: //top
			if (dir!=2)
				return false;
			break;

		case 3: //right
			if (dir!=3)
				return false;
			break;

		case 4: //bottom
			if (dir!=0)
				return false;
			break;

		case 5: //left
			if (dir!=1)
				return false;
			break;

		case 6: //topRight
			if (dir!=2 && dir!=3)
				return false;
			break;

		case 7: //bottomRight
			if (dir!=0 && dir!=3)
				return false;
			break;

		case 8: //bottomLeft
			if (dir!=0 && dir!=1)
				return false;
			break;

		case 9: //topLeft
			if (dir!=1 && dir!=2)
				return false;
			break;

		case 10: //topBottom
			if (dir!=0 && dir!=2)
				return false;

		case 11: //leftRight
			if (dir!=1 && dir!=3)
				return false;

		case 12: //noTop
			if (dir==2)
				return false;
			break;

		case 13: //noRight
			if (dir==3)
				return false;
			break;

		case 14: //noBottom
			if (dir==0)
				return false;
			break;

		case 15: //noLeft
			if (dir==1)
				return false;
			break;

		default:
			cout << "Warning: Collision of unknown type encountered in map: " << uniqueName << endl;
		}
	}
	return true;
}

bool Map::spaceFree(FloatRect box) {
    if (box.left<0 || box.top<0 || box.left+box.width>=size.x*32 || box.top+box.height>=size.y*32)
		return false;

    vector<Vector2i> tiles = Map::getCoveredTiles(box);
    for (unsigned int i = 0; i<tiles.size(); ++i) {
		if (tiles[i].x<0 || tiles[i].y<0 || tiles[i].x>=size.x || tiles[i].y>=size.y)
			return false;
		if (collisions(tiles[i].x,tiles[i].y)!=1)
			return false;
    }
    return true;
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

void Map::addLight(Light l) {
	lights.push_back(l);
}

Light* Map::getLight(int x, int y) {
	for (unsigned int i = 0; i<lights.size(); ++i) {
		int d = (lights[i].position.x-x)*(lights[i].position.x-x)+(lights[i].position.y-y)*(lights[i].position.y-y);
		if (d<lights[i].radius*lights[i].radius) {
			return &lights[i];
		}
    }
    return nullptr;
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

void Map::addEntitySpawn(EntitySpawn ps) {
	entitySpawns.push_back(ps);
}

EntitySpawn* Map::getEntitySpawn(int x, int y) {
	for (unsigned int i = 0; i<entitySpawns.size(); ++i) {
		if (entitySpawns[i].position.coords.x==x && entitySpawns[i].position.coords.y==y)
			return &entitySpawns[i];
	}
	return nullptr;
}

void Map::removeEntitySpawn(int x, int y) {
	for (unsigned int i = 0; i<entitySpawns.size(); ++i) {
		if (entitySpawns[i].position.coords.x==x && entitySpawns[i].position.coords.y==y) {
			entitySpawns.erase(entitySpawns.begin()+i);
			--i;
		}
	}
}

void Map::addItem(int itemId, Vector2i position) {
	MapItem item;
	item.itemId = itemId;
	item.mapId = maxMapItemId;
	item.position = position;
	maxMapItemId++;
	EntityPosition pos;
	pos.coords = Vector2f(position);
	pos.mapName = uniqueName;
	Entity::Ptr ie = ItemEntity::create(item.itemId, pos);
	EntityManager::get()->add(ie);
	items.push_back(item);
}

MapItem* Map::getItem(Vector2i position) {
	for (unsigned int i = 0; i<items.size(); ++i) {
		if (abs(items[i].position.x-position.x)<=32 && abs(items[i].position.y-position.y)<=32)
			return &items[i];
	}
	return nullptr;
}

void Map::updateItem(MapItem* orig) {
	EntityPosition pos = orig->ie->getPosition();
	EntityManager::get()->remove(orig->ie);
	orig->ie = ItemEntity::create(orig->itemId, pos);
	EntityManager::get()->add(orig->ie);
}

void Map::removeItem(Vector2i position) {
	for (unsigned int i = 0; i<items.size(); ++i) {
		if (abs(items[i].position.x-position.x)<=32 && abs(items[i].position.y-position.y)<=32) {
			EntityManager::get()->remove(items[i].ie);
			items.erase(items.begin()+i);
			--i;
		}
	}
}

void Map::addMapSpawner(MapSpawner sp) {
    spawners.push_back(sp);
}

MapSpawner* Map::getMapSpawner(int x, int y) {
	for (unsigned int i = 0; i<spawners.size(); ++i) {
		if (spawners[i].position.x==x && spawners[i].position.y==y)
			return &spawners[i];
	}
	return nullptr;
}

void Map::removeMapSpawner(int x, int y) {
	for (unsigned int i = 0; i<spawners.size(); ++i) {
		if (spawners[i].position.x==x && spawners[i].position.y==y) {
			spawners.erase(spawners.begin()+i);
			--i;
		}
	}
}

void Map::removeAllLights() {
	lights.clear();
}

void Map::setLightingOverride(int o) {
	ambientLightOverride = o;
}

int Map::getLightOverride() {
	return ambientLightOverride;
}

void Map::editTile(int x, int y, int layer, int nId, bool isAnim) {
    if (layer<0 || layer>=signed(layers.size()) || x<0 || x>=size.x || y<0 || y>=size.y)
        return;

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
		syncAnimTable();
		layers[layer](x,y).delA = !tileset.getAnimation(nId)->isLooping();
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
	for (unsigned int i = 0; i<layers.size(); ++i) {
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
