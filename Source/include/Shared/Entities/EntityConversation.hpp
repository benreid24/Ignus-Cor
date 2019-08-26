#ifndef ENTITYCONVERSATION_HPP
#define ENTITYCONVERSATION_HPP

#include <string>
#include <memory>
#include <map>
#include "Shared/Entities/Entity.hpp"
#include "Shared/Util/Util.hpp"

class EntityBubble;

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
    public:
        typedef std::shared_ptr<Node> Ptr;

        /**
         * Destructor
         */
        virtual ~Node() = default;

        /**
         * Updates the proper EntityBubble's as need be with node specific details
         */
        virtual void apply(EntityBubble& playerbubble, EntityBubble& owner) = 0;

        /**
         * Returns a pointer to the next node, or nullptr if the node is not finished
         */
        virtual Node* getNext() = 0;
    };
    class TalkNode;
    class ActionNode;
    class ScriptNode;
    class CheckNode;
    class ChoiceNode;

public:
    /**
     * Creates an empty conversation
     */
    EntityConversation(Entity* owner);

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
    std::map<std::string, Node::Ptr> nodes;
    Node* currentNode;
};

#endif // ENTITYCONVERSATION_HPP
