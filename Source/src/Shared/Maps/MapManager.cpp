#include "Shared/Maps/MapManager.hpp"
#include "Game/Core/Game.hpp"
#include "Shared/Entities/EntityManager.hpp"
#include "Shared/Media/Playlist.hpp"
#include "Shared/Media/SoundEngine.hpp"
using namespace std;
using namespace sf;

namespace {
    const int mapTimeout = 10 * 60*1000;
}

MapManager::MapManager(Game* g, EntityManager* em, SoundEngine* se, Playlist* plst) {
    game = g;
    entityManager = em;
    soundEngine = se;
    playlist = plst;
}

void MapManager::setPlayer(Entity::Ptr p) {
	player = p;
}

void MapManager::loadMap(string mapfile) {
    if (maps.find(mapfile)==maps.end()) {
        MapHolder temp;
        temp.lastActiveTime = timer.getElapsedTime().asMilliseconds();
        temp.mapdata = shared_ptr<Map>(new Map(mapfile, tileset, entityManager, soundEngine, player.get(), playlist));
        maps.insert(make_pair(mapfile, temp)).first;
        if (maps.size()==1)
			activeMap = mapfile;
    }
}

void MapManager::mapChange(Entity* e, string mapfile, string spawn) {
    loadMap(mapfile);
    map<string,MapHolder>::iterator i = maps.find(mapfile);
    if (e==player.get())
        activeMap = mapfile;
	i->second.mapdata->spawnEntity(e, spawn);
}

void MapManager::update() {
    maps[activeMap].lastActiveTime = timer.getElapsedTime().asMilliseconds();

    vector<string> todelete;
    for (map<string,MapHolder>::iterator i = maps.begin(); i!=maps.end(); ++i) {
        i->second.mapdata->update();
        if (timer.getElapsedTime().asMilliseconds()-i->second.lastActiveTime >= mapTimeout)
            todelete.push_back(i->first);
    }
    for (unsigned int i = 0; i<todelete.size(); ++i) {
        maps.erase(todelete[i]);
        //TODO - Remove Entity objects in EntityManager that are in the deleted map
    }
}

void MapManager::render() {
    maps[activeMap].mapdata->draw(game->window);
}

void MapManager::updateRenderPosition(Vector2f playerCoords) {
	maps[activeMap].mapdata->setRenderPosition(playerCoords);
}
