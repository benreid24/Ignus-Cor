#ifndef ENTITYCONVERSATION_HPP
#define ENTITYCONVERSATION_HPP

#include <string>
#include <memory>
#include <map>
#include "Shared/Entities/Entity.hpp"
#include "Shared/Util/Util.hpp"

class EntityBubble;
class EntityConversationNodeFactory;

/**
 * Conversation tree for AI entities to converse with the player
 *
 * \ingroup Entity
 */
class EntityConversation {
    /**
     * Base node for a conversation node. Provides the interface
     */
    class Node {
        std::string name;

    public:
        typedef std::shared_ptr<Node> Ptr;

        /**
         * Constructs the node with the given name
         */
        Node(const std::string& nm) : name(nm) {}

        /**
         * Destructor
         */
        virtual ~Node() = default;

        /**
         * Returns the name of the Node
         */
        const std::string& getName() const { return name; }

        /**
         * Updates the proper EntityBubble's as need be with node specific details
         */
        virtual void apply(EntityBubble& playerbubble, EntityBubble& owner) = 0;

        /**
         * Returns a pointer to the next node, or nullptr if the node is not finished
         */
        virtual Node* getNext() = 0;
    };

    friend class EntityConversationNodeFactory;

public:
    /**
     * Creates an empty conversation
     */
    EntityConversation(Entity* player, Entity* owner);

    /**
     * Loads the conversation from the given file
     */
    void load(File& file);

    /**
     * Saves the conversation to the given file
     */
    void save(File& file);

    /**
     * Starts the conversation if interacted with
     */
    void activate();

    /**
     * Updates the conversation if it is in progress
     */
    void update();

private:
    Entity* owner;
    Entity* player;
    std::list<Node::Ptr> nodes;
    std::map<std::string, Node::Ptr> nodeMap;
    Node::Ptr exitNode;
    Node* currentNode;
};

#endif // ENTITYCONVERSATION_HPP
