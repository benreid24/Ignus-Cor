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
     * Stops the conversation
     */
    void stop();

    /**
     * Updates the conversation if it is in progress
     */
    void update();

private:
    class Node;

    /**
     * Returns a pointer to the node with the given name, or nullptr if none found
     */
    Node* getNode(Node* node, const std::string& name);

    /**
     * Returns a pointer to the node after the given node
     */
    Node* getFollowingNode(Node* node);

    /**
     * Base node for a conversation node. Provides the interface
     */
    class Node {
        std::string name;

    protected:
        Node* getNode(EntityConversation* conv, const std::string& nm) { return conv->getNode(this, nm); }

    public:
        typedef std::shared_ptr<Node> Ptr;

        /**
         * Constructs an empty node
         */
        Node() : name("TEST") {}

        /**
         * Constructs the node with the given name
         */
        Node(const std::string& nm) : name(nm) {}

        /**
         * Constructs the node by loading the name from the input file
         */
        Node(File& input) : name(input.getString()) {}

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
        virtual void apply(EntityConversation* conv, EntityBubble& playerbubble, EntityBubble& owner) = 0;

        /**
         * Reset internal state for new conversation
         */
        virtual void reset() = 0;

        /**
         * Returns a pointer to the next node, or nullptr if the node is not finished
         */
        virtual Node* getNext(EntityConversation* conv) = 0;
    };

    friend class Node;
    friend class EntityConversationNodeFactory;

private:
    Entity* owner;
    Entity* player;
    std::list<Node::Ptr> nodes;
    std::map<std::string, Node::Ptr> nodeMap;
    Node::Ptr exitNode;
    Node* currentNode;
    bool active;
};

#endif // ENTITYCONVERSATION_HPP
