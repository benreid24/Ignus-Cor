#include "Shared/Entities/EntityConversation.hpp"
#include "Shared/Entities/EntityConversationNodeFactory.hpp"
using namespace std;

namespace {
const string testString = "This is for testing only. This conversation has been created but not loaded from a file. This should not be visible";
}

EntityConversation::EntityConversation(Entity* p, Entity* o)
: owner(o), player(p), currentNode(nullptr), active(false) {
    nodes.emplace_back(EntityConversationNodeFactory::getTestNode(testString));
    nodes.emplace_back(EntityConversationNodeFactory::getExitNode());
    for (auto node = nodes.begin(); node!=nodes.end(); ++node) {
        nodeMap.emplace((*node)->getName(), *node);
    }
}

void EntityConversation::activate() {
    if (nodes.size() > 0 && !active) {
        currentNode = nodes.begin()->get();
        active = true;
        for (auto i = nodes.begin(); i!=nodes.end(); ++i) {
            i->get()->reset();
        }
    }
}

void EntityConversation::stop() {
    active = false;
}

void EntityConversation::update() {
    if (!active)
        return;
    if (currentNode == nullptr) {
        active = false;
        return;
    }

    while (currentNode->getNext(this) != nullptr && currentNode != currentNode->getNext(this))
        currentNode = currentNode->getNext(this);
    if (currentNode != nullptr)
        currentNode->apply(this, player->getBubble(), owner->getBubble());
    else
        active = false;
}

void EntityConversation::load(File& file) {
    nodes = EntityConversationNodeFactory::load(file);
    exitNode = EntityConversationNodeFactory::getExitNode();
    nodes.push_back(exitNode);
    for (auto node = nodes.begin(); node!=nodes.end(); ++node) {
        nodeMap.emplace((*node)->getName(), *node);
    }
    if (nodes.size() > 0)
        currentNode = nodes.begin()->get();
    else
        currentNode = exitNode.get();
}

void EntityConversation::save(File& file) {
    //TODO - save
}

EntityConversation::Node* EntityConversation::getNode(Node* node, const string& name) {
    if (name.size() == 0 || name=="NEXT")
        return getFollowingNode(node);
    auto i = nodeMap.find(name);
    if (nodeMap.end() == i)
        return nullptr;
    return i->second.get();
}

EntityConversation::Node* EntityConversation::getFollowingNode(Node* node) {
    for (auto i = nodes.begin(); i!=nodes.end(); ++i) {
        if (i->get() == node) {
            if (distance(i, nodes.end()) > 1)
                return (++i)->get();
            return nullptr;
        }
    }
    return nullptr;
}
