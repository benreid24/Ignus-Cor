#include "Shared/Entities/EntityManager.hpp"
#include "Game/Core/Game.hpp"
using namespace sf;
using namespace std;

EntityManager::EntityManager() {
    //ctor
}

EntityManager::~EntityManager() {
	clear();
}

void EntityManager::updatePosition(Entity* e, EntityPosition oldPos) {
	int ly = oldPos.coords.y/32;
	int cy = e->getPosition().coords.y/32;
	string lmap = oldPos.mapName;
	string cmap = e->getPosition().mapName;

	if (ySortedEntities.find(lmap)==ySortedEntities.end() || ySortedEntities.find(cmap)==ySortedEntities.end()) {
		cout << "Warning: Entity position in invalid map: " << lmap << " -> " << cmap << endl;
		return;
	}
	if (ly<0 || ly>=signed(ySortedEntities[lmap].size()) || cy<0 || cy>=signed(ySortedEntities[cmap].size())) {
		cout << "Warning: Entity y-position update " << ly << " to " << cy << " is out of range\n";
		return;
	}

	if (ly!=cy || lmap!=cmap) {
		//Remove from old row
		Entity::Ptr fe;
		vector<Entity::Ptr>* ymap = &(ySortedEntities[lmap][ly]);
		for (unsigned int i = 0; i<ymap->size(); ++i) {
			if (ymap->at(i).get()==e) {
				fe = ymap->at(i);
				ymap->erase(ymap->begin()+i);
				break;
			}
		}
		//Add to new row
		ySortedEntities[cmap][cy].push_back(fe);
	}
}

bool EntityManager::spaceFree(Entity* e, EntityPosition space, Vector2f size) {
	auto i = ySortedEntities.find(space.mapName);
	int spaceY = space.coords.y/32;
	FloatRect box(space.coords, size);

	if (i!=ySortedEntities.end()) {
		for (int y = spaceY - 2; y <= spaceY + 2; ++y) {
			if (y>=0 && y<signed(i->second.size())) {
                for (unsigned int j = 0; j<i->second[y].size(); ++j) {
					if (box.intersects(i->second[y][j]->getBoundingBox()) && i->second[y][j].get()!=e)
						return false;
                }
			}
		}
		return true;
	}
	else
		return false;
}

bool EntityManager::canMove(Entity* e, EntityPosition oldPos, EntityPosition newPos, Vector2f size) {
    #ifdef GAME
    if (oldPos.mapName==newPos.mapName)
		return Game::get()->mapManager.movementValid(oldPos, newPos, size) && spaceFree(e, newPos, size);
    return Game::get()->mapManager.spaceFree(newPos, size) && spaceFree(e, newPos, size);
    #endif
    return false;
}

void EntityManager::clear() {
	ySortedEntities.clear();
	entities.clear();
}

vector<vector<Entity::Ptr> >& EntityManager::getYSorted(string mapname) {
	auto i = ySortedEntities.find(mapname);
	if (i==ySortedEntities.end()) {
		cout << "CRITICAL: Map requesting non-existent ysorted array by name: " << mapname << endl;
		return ySortedEntities.begin()->second;
	}
	return i->second;
}

void EntityManager::registerMap(string mapname, int height) {
	ySortedEntities.erase(mapname);
	ySortedEntities[mapname] = vector<vector<Entity::Ptr> >(height);
	vector<vector<Entity::Ptr> >* ymap = &ySortedEntities[mapname];
	for (unsigned int i = 0; i<entities.size(); ++i) {
		int y = entities[i]->getPosition().coords.y/32;
		if (y>=0 && y<height && entities[i]->getPosition().mapName==mapname)
			ymap->at(i).push_back(entities[i]);
	}
}

void EntityManager::add(Entity::Ptr e) {
	if (ySortedEntities.find(e->getPosition().mapName)==ySortedEntities.end()) {
		cout << "CRITICAL: Tried to insert an Entity into non-existent map: " << e->getPosition().mapName << endl;
		return;
	}

	entities.push_back(e);
	int y = e->getPosition().coords.y/32;
	if (y>=0 && y<signed(ySortedEntities[e->getPosition().mapName].size()))
		ySortedEntities[e->getPosition().mapName][y].push_back(e);
	else
		cout << "ERROR: Tried to add entity to map " << e->getPosition().mapName << " but position was out of range" << endl;
}

void EntityManager::remove(Entity::Ptr e) {
	int y = e->getPosition().coords.y/32;
	for (unsigned int i = 0; i<entities.size(); ++i) {
		if (entities[i]==e) {
			entities.erase(entities.begin()+i);
            --i;
		}
	}
	auto pos = ySortedEntities.find(e->getPosition().mapName);
	if (pos!=ySortedEntities.end()) {
		if (y>=0 && y<signed(pos->second.size())) {
			for (unsigned int i = 0; i<pos->second[y].size(); ++i) {
				if (pos->second[y][i]==e) {
					pos->second[y].erase(pos->second[y].begin()+i);
					--i;
				}
			}
		}
	}
}

void EntityManager::remove(string name, string type) {
	for (unsigned int i = 0; i<entities.size(); ++i) {
		if (entities[i]->getName()==name && (entities[i]->getType()==type || type.size()==0)) {
            remove(entities[i]);
            --i;
		}
	}
}

void EntityManager::update() {
	for (unsigned int i = 0; i<entities.size(); ++i) {
		entities[i]->update();
	}
}

void EntityManager::updateRenderPosition(sf::Vector2f playerCoords) {
    #ifdef GAME
	Game::get()->mapManager.updateRenderPosition(playerCoords);
	#endif
}
