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

EntityManager* EntityManager::get() {
    static EntityManager manager;
    return &manager;
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

Entity::List EntityManager::getEntitiesInSpace(const string& mapName, const FloatRect& box) {
	Entity::List ret;
	auto i = ySortedEntities.find(mapName);
	int spaceY = box.top/32;

	if (i!=ySortedEntities.end()) {
		for (int y = spaceY - 2; y <= spaceY + 2; ++y) {
			if (y>=0 && y<signed(i->second.size())) {
                for (unsigned int j = 0; j<i->second[y].size(); ++j) {
					if (box.intersects(i->second[y][j]->getBoundingBox()))
						ret.push_back( i->second[y][j]);
                }
			}
		}
	}
	return ret;
}

bool EntityManager::spaceFree(Entity* e, EntityPosition space, Vector2f size) {
	auto i = ySortedEntities.find(space.mapName);
	int spaceY = space.coords.y/32;
	FloatRect box(space.coords, size);

	if (i!=ySortedEntities.end()) {
		for (int y = spaceY - 2; y <= spaceY + 2; ++y) {
			if (y>=0 && y<signed(i->second.size())) {
                for (unsigned int j = 0; j<i->second[y].size(); ++j) {
					if (box.intersects(i->second[y][j]->getBoundingBox()) && i->second[y][j].get()!=e && i->second[y][j]->collidesWithOtherEntities())
						return false;
                }
			}
		}
		return true;
	}
    return false;
}

bool EntityManager::canMove(Entity* e, EntityPosition oldPos, EntityPosition newPos, Vector2f size) {
    if (oldPos.mapName==newPos.mapName)
		return MapManager::get()->movementValid(oldPos, newPos, size) && spaceFree(e, newPos, size);
    return MapManager::get()->spaceFree(newPos, size) && spaceFree(e, newPos, size);
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
	entityPointerMap[e.get()] = e;
	int y = e->getPosition().coords.y/32;
	if (y>=0 && y<signed(ySortedEntities[e->getPosition().mapName].size()))
		ySortedEntities[e->getPosition().mapName][y].push_back(e);
	else
		cout << "ERROR: Tried to add entity to map " << e->getPosition().mapName << " but position was out of range" << endl;
}

void EntityManager::doDelete(Entity::Ptr e) {
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
	entityPointerMap.erase(e.get());
}

void EntityManager::remove(Entity::Ptr e) {
    Lock delLock(deleteLock);
    entityDeleteQueue.push_back(e);
}

void EntityManager::remove(string name, string type) {
	for (unsigned int i = 0; i<entities.size(); ++i) {
		if (entities[i]->getName()==name && (entities[i]->getType()==type || type.size()==0)) {
            remove(entities[i]);
            --i;
		}
	}
}

Entity::Ptr EntityManager::doInteract(Entity* interactor, bool notify) {
    Entity::Ptr ent = getEntityPtr(interactor);
    if (ent.get() == nullptr) {
        cout << "Error: " << interactor->getIdString() << " is unregistered with EntityManager\n";
        return Entity::Ptr(nullptr);
    }

    FloatRect box = ent->getInteractBox();
    vector<vector<Entity::Ptr> >& ents = getYSorted(ent->getPosition().mapName);
    unsigned int minY = (box.top-box.height/2)/32;
    unsigned int maxY = (box.top+box.height*1.5)/32;

    for (unsigned int y = minY; y<=maxY; ++y) {
        if (y>=0 && y<ents.size()) {
            for (unsigned int i = 0; i<ents[y].size(); ++y) {
                if (box.intersects(ents[y][i]->getBoundingBox())) {
                    if (notify)
                        ents[y][i]->notifyInteracted(ent);
                    return ents[y][i];
                }
            }
        }
    }
    return Entity::Ptr(nullptr);
}

void EntityManager::update() {
	for (unsigned int i = 0; i<entities.size(); ++i) {
		entities[i]->update();
	}

	Lock delLock(deleteLock);
	for (auto i = entityDeleteQueue.begin(); i!=entityDeleteQueue.end(); ++i) {
        doDelete(*i);
	}
	entityDeleteQueue.clear();
}

Entity::Ptr EntityManager::getEntityPtr(Entity* ent) {
    if (entityPointerMap.find(ent)!=entityPointerMap.end())
        return entityPointerMap[ent].lock();
    return Entity::Ptr(nullptr);
}
