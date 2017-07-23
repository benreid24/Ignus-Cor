#include "Shared/Entities/EntityManager.hpp"
using namespace sf;
using namespace std;

//TODO - define EntityManager
EntityManager::EntityManager() {
	//
}

EntityManager::~EntityManager() {
	//
}

vector<vector<Entity*> >& EntityManager::getYSorted() {
	return ySortedEntities;
}

void EntityManager::setMapHeight(int height) {
	ySortedEntities.clear();
	ySortedEntities.resize(height);
	for (unsigned int i = 0; i<entities.size(); ++i) {
		int y = entities[i]->getPosition().y/32;
		if (y>=0 && y<height)
			ySortedEntities[y].push_back(entities[i]);
	}
}
