#include "Shared/Entities/EntityConversation.hpp"
#include "Shared/Entities/EntityConversationNodeFactory.hpp"
using namespace std;

EntityConversation::EntityConversation(Entity* p, Entity* o)
: owner(o), player(p), currentNode(nullptr) {}

void EntityConversation::update() {
    while (currentNode->getNext(this) != nullptr && currentNode != currentNode->getNext(this))
        currentNode = currentNode->getNext(this);
    currentNode->apply(this, player->getBubble(), owner->getBubble());
}

void EntityConversation::load(File& file) {
    nodes = EntityConversationNodeFactory::load(file);
    for (auto node = nodes.begin(); node!=nodes.end(); ++node) {
        nodeMap.emplace((*node)->getName(), *node);
    }
    exitNode = EntityConversationNodeFactory::getExitNode();
    if (nodes.size() > 0)
        currentNode = nodes.begin()->get();
    else
        currentNode = exitNode.get();
}

void EntityConversation::save(File& file) {
    //TODO - save
}

EntityConversation::Node* EntityConversation::getNode(const string& name) {
    auto i = nodeMap.find(name);
    if (nodeMap.end() == i)
        return nullptr;
    return i->second.get();
}
