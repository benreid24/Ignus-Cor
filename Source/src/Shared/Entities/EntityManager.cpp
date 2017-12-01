#include "Shared/Entities/EntityManager.hpp"
using namespace sf;
using namespace std;

EntityManager::EntityManager() {
	//default
}

EntityManager::~EntityManager() {
	clear();
}

void EntityManager::updatePosition(Entity::Ptr e, float lastY, float curY) {
	int ly = lastY/32;
	int cy = curY/32;

	if (ly<0 || ly>=signed(ySortedEntities.size()) || cy<0 || cy>=signed(ySortedEntities.size())) {
		cout << "Warning: Entity y-position update " << ly << " to " << cy << " is out of range\n";
		return;
	}

	if (ly!=cy) {
		//Remove from old row
		for (unsigned int i = 0; i<ySortedEntities[ly].size(); ++i) {
			if (ySortedEntities[ly][i]==e) {
				ySortedEntities[ly].erase(ySortedEntities[ly].begin()+i);
				break;
			}
		}
		//Add to new row
		ySortedEntities[cy].push_back(e);
	}
}

void EntityManager::clear() {
	ySortedEntities.clear();
	entities.clear();
}

vector<vector<Entity::Ptr> >& EntityManager::getYSorted() {
	return ySortedEntities;
}

void EntityManager::setMapHeight(int height) {
	ySortedEntities.clear();
	ySortedEntities.resize(height);
	for (unsigned int i = 0; i<entities.size(); ++i) {
		int y = entities[i]->getPosition().coords.y/32;
		if (y>=0 && y<height)
			ySortedEntities[y].push_back(entities[i]);
	}
}

void EntityManager::add(Entity::Ptr e) {
	entities.push_back(e);
	int y = e->getPosition().coords.y/32;
	if (y>=0 && y<signed(ySortedEntities.size()))
		ySortedEntities[y].push_back(e);
}

void EntityManager::remove(Entity::Ptr e) {
	int y = e->getPosition().coords.y/32;
	for (unsigned int i = 0; i<entities.size(); ++i) {
		if (entities[i]==e) {
			entities.erase(entities.begin()+i);
            --i;
		}
	}
    if (y>=0 && y<signed(ySortedEntities.size())) {
		for (unsigned int i = 0; i<ySortedEntities[y].size(); ++i) {
			if (ySortedEntities[y][i]==e) {
				ySortedEntities[y].erase(ySortedEntities[y].begin()+i);
				--i;
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
