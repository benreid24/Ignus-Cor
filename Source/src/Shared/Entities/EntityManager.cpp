#include "Shared/Entities/EntityManager.hpp"
#include "Game/Core/Game.hpp"
#include "Shared/Entities/Instances/NpcEntity.hpp"
using namespace sf;
using namespace std;

EntityManager::EntityManager() {
    Entity::Ptr test = NpcEntity::create("Test Npc",
                                         EntityPosition(Vector2f(680, 680), "test.map", EntityPosition::Up),
                                         "Entities/GirlPlayer/Walk",
                                         "Entities/GirlPlayer/Run");
    add(test);
}

EntityManager::~EntityManager() {
	clear();
}

EntityManager& EntityManager::get() {
    static EntityManager manager;
    return manager;
}

void EntityManager::updatePosition(Entity* e, EntityPosition oldPos) {
    Entity::Ptr ent = getEntityPtr(e);
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
		if (e->isYSortRendered()) {
            //Remove from old row
            vector<Entity::Ptr>* ymap = &(ySortedEntities[lmap][ly]);
            for (unsigned int i = 0; i<ymap->size(); ++i) {
                if (ymap->at(i).get()==e) {
                    ymap->erase(ymap->begin()+i);
                    break;
                }
            }
            //Add to new row
            ySortedEntities[cmap][cy].push_back(ent);
		}
		else if (lmap != cmap) {
            auto iter = find(topEntities[lmap].begin(), topEntities[lmap].end(), ent);
            if (iter != topEntities[lmap].end())
                topEntities[lmap].erase(iter);
            topEntities[cmap].push_back(ent);
		}
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
					if (box.intersects(i->second[y][j]->getBoundingBox()) && i->second[y][j]->collidesWithOtherEntities())
						ret.push_back(i->second[y][j]);
                }
			}
		}
	}
	return ret;
}

bool EntityManager::spaceFree(Entity* e, EntityPosition space, Vector2f size) {
    if (!e->collidesWithOtherEntities())
        return false;

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
		return MapManager::get().movementValid(oldPos, newPos, size) && spaceFree(e, newPos, size);
    return MapManager::get().spaceFree(newPos, size) && spaceFree(e, newPos, size);
}

void EntityManager::clear() {
    Lock lock(entityLock);
    entityAddQueue.clear();
    entityDeleteQueue.clear();
	ySortedEntities.clear();
	topEntities.clear();
	entities.clear();
}

vector<vector<Entity::Ptr> >& EntityManager::getYSortedEntities(string mapname) {
	auto i = ySortedEntities.find(mapname);
	if (i==ySortedEntities.end()) {
		cout << "CRITICAL: Map requesting non-existent ysorted array by name: " << mapname << endl;
		return ySortedEntities.begin()->second;
	}
	return i->second;
}

vector<Entity::Ptr>& EntityManager::getTopEntities(string mapname) {
    auto i = topEntities.find(mapname);
	if (i==topEntities.end()) {
		cout << "CRITICAL: Map requesting non-existent ysorted array by name: " << mapname << endl;
		return topEntities.begin()->second;
	}
	return i->second;
}

void EntityManager::registerMap(string mapname, int height) {
	ySortedEntities.erase(mapname);
	topEntities.erase(mapname);
	ySortedEntities[mapname] = vector<vector<Entity::Ptr> >(height);
	topEntities[mapname] = vector<Entity::Ptr>();
	vector<vector<Entity::Ptr> >* ymap = &ySortedEntities[mapname];
	for (unsigned int i = 0; i<entities.size(); ++i) {
		int y = entities[i]->getPosition().coords.y/32;
		if (y>=0 && y<height && entities[i]->getPosition().mapName==mapname) {
            if (entities[i]->isYSortRendered())
                ymap->at(i).push_back(entities[i]);
			else
                topEntities[mapname].push_back(entities[i]);
		}
	}
}

void EntityManager::add(Entity::Ptr e) {
    Lock lock(entityLock);
    entityAddQueue.push_back(e);
}

void EntityManager::doAdd(Entity::Ptr e) {
	if (ySortedEntities.find(e->getPosition().mapName)==ySortedEntities.end()) {
		cout << "CRITICAL: Tried to insert an Entity into non-existent map: " << e->getPosition().mapName << endl;
		return;
	}

	entities.push_back(e);
	entityPointerMap[e.get()] = e;
	int y = e->getPosition().coords.y/32;
	if (y>=0 && y<signed(ySortedEntities[e->getPosition().mapName].size())) {
        if (e->isYSortRendered())
            ySortedEntities[e->getPosition().mapName][y].push_back(e);
        else
            topEntities[e->getPosition().mapName].push_back(e);
	}
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
		for (unsigned int i = 0; i<topEntities[e->getPosition().mapName].size(); ++i) {
            if (topEntities[e->getPosition().mapName][i]==e) {
                topEntities[e->getPosition().mapName].erase(topEntities[e->getPosition().mapName].begin()+i);
                --i;
            }
		}
	}

	entityPointerMap.erase(e.get());
}

void EntityManager::remove(Entity::Ptr e) {
    Lock lock(entityLock);
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

void EntityManager::remove(Entity* e) {
    remove(getEntityPtr(e));
}

void EntityManager::update() {
	for (unsigned int i = 0; i<entities.size(); ++i) {
		entities[i]->update();
	}

	Lock lock(entityLock);
	for (auto i = entityDeleteQueue.begin(); i!=entityDeleteQueue.end(); ++i) {
        doDelete(*i);
	}
	entityDeleteQueue.clear();
	for (auto i = entityAddQueue.begin(); i!=entityAddQueue.end(); ++i) {
        doAdd(*i);
	}
	entityAddQueue.clear();
}

Entity::Ptr EntityManager::getEntityPtr(Entity* ent) {
    if (entityPointerMap.find(ent)!=entityPointerMap.end())
        return entityPointerMap[ent].lock();
    return Entity::Ptr(nullptr);
}
