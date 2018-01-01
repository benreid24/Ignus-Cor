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

MapManager::MapManager() {
    game = nullptr;
    player = nullptr;
    entityManager = nullptr;
    playlist = nullptr;
    soundEngine = nullptr;
}

void MapManager::setPointers(Game* g, Entity* p, EntityManager* em, SoundEngine* se, Playlist* plst) {
    game = g;
    player = p;
    entityManager = em;
    soundEngine = se;
    playlist = plst;
}

void MapManager::mapChange(Entity* e, string mapfile, string spawn) {
    map<string,MapHolder>::iterator i = maps.find(mapfile);
    if (i==maps.end()) {
        MapHolder temp;
        temp.lastActiveTime = timer.getElapsedTime().asMilliseconds();
        temp.mapdata = shared_ptr<Map>(new Map(mapfile, tileset, entityManager, soundEngine, player, playlist));
        i = maps.insert(make_pair(mapfile, temp)).first;
    }
    if (e==player)
        activeMap = mapfile;
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
