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
