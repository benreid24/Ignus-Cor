#include "Shared/Maps/MapManager.hpp"
#include "Game/Core/Game.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Media/Playlist.hpp"
#include "Shared/Media/SoundEngine.hpp"
#include "Shared/Util/Timer.hpp"
using namespace std;
using namespace sf;

namespace {
    const int mapTimeout = 10 * 60*1000;
}

MapManager::MapManager() {
    //ctor
}

MapManager& MapManager::get() {
    static MapManager manager;
    return manager;
}

void MapManager::setPlayer(Entity::Ptr p) {
	player = p;
}

bool MapManager::movementValid(EntityPosition oldPos, EntityPosition newPos, Vector2f size) {
	FloatRect oldBox(oldPos.coords, size);
	FloatRect newBox(newPos.coords, size);

	if (oldPos.mapName!=newPos.mapName)
		return spaceFree(newPos, size);
	auto i = maps.find(oldPos.mapName);
	if (i!=maps.end())
		return i->second.mapdata->spaceFree(oldBox, newBox);
	return false;
}

bool MapManager::spaceFree(EntityPosition space, Vector2f size) {
	auto i = maps.find(space.mapName);
	if (i!=maps.end())
		return i->second.mapdata->spaceFree(FloatRect(space.coords, size));
	return false;
}

void MapManager::loadMap(string mapfile) {
    if (maps.find(mapfile)==maps.end()) {
        MapHolder temp;
        temp.lastActiveTime = Timer::get().timeElapsedMilliseconds();
        temp.mapdata = shared_ptr<Map>(new Map(mapfile, tileset, player));
        maps.insert(make_pair(mapfile, temp)).first;
        if (maps.size()==1)
			activeMap = mapfile;
    }
}

void MapManager::mapChange(Entity::Ptr e, string mapfile, string spawn) {
    loadMap(mapfile);
    map<string,MapHolder>::iterator i = maps.find(mapfile);
    if (e==player)
        activeMap = mapfile;
	i->second.mapdata->spawnEntity(e, spawn);
}

void MapManager::update() {
    maps[activeMap].lastActiveTime = Timer::get().timeElapsedMilliseconds();

    vector<string> todelete;
    for (map<string,MapHolder>::iterator i = maps.begin(); i!=maps.end(); ++i) {
        i->second.mapdata->update();
        if (Timer::get().timeElapsedMilliseconds()-i->second.lastActiveTime >= mapTimeout)
            todelete.push_back(i->first);
    }
    for (unsigned int i = 0; i<todelete.size(); ++i) {
        maps.erase(todelete[i]);
        //TODO - Remove Entity objects in EntityManager that are in the deleted map
    }
}

void MapManager::registerEntityMovement(Entity::Ptr ent, FloatRect oldBox) {
    auto i = maps.find(ent->getPosition().mapName);
    if (i!=maps.end())
        i->second.mapdata->moveOntoTile(ent, oldBox);
    else {
        cout << "Error: " << ent->getIdString() << " in unloaded map " << ent->getPosition().mapName << ", deleting\n";
        EntityManager::get().remove(ent);
    }
}

void MapManager::render() {
    #ifdef GAME
    maps[activeMap].mapdata->draw(Game::get().window);
    #endif
}

void MapManager::updateRenderPosition(Vector2f playerCoords) {
	maps[activeMap].mapdata->setRenderPosition(playerCoords);
}

string MapManager::getCurrentMap() {
    return activeMap;
}
