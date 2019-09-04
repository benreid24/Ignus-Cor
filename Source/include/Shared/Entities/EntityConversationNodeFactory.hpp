#ifndef ENTITYCONVERSATIONNODEFACTORY_HPP
#define ENTITYCONVERSATIONNODEFACTORY_HPP

#include <Shared/Entities/EntityConversation.hpp>

/**
 * Factory class to parse and load nodes for EntityConversation
 * Provides Node subclasses and a static parser method
 *
 * \ingroup Entity
 */
class EntityConversationNodeFactory {
public:
    /**
     * Returns a list of nodes from the input file
     */
    static std::list<EntityConversation::Node::Ptr> load(File& input);

    /**
     * Creates and returns an exit node
     */
    static EntityConversation::Node::Ptr getExitNode();

private:
    /**
     * Simply says a single line
     */
    class TalkNode : public EntityConversation::Node {
        std::string say;
        std::string nextNode;

    public:
        /**
         * Constructs the node with data from the input file
         */
        TalkNode(File& input);

        /**
         * Destructor
         */
        virtual ~TalkNode() = default;

        /**
         * Outputs the text to the given bubble
         */
        virtual void apply(EntityBubble& playerbubble, EntityBubble& owner) override;

        /**
         * Returns a pointer to the next node, or nullptr if the node is not finished
         */
        virtual Node* getNext() override;
    };

    /**
     * Presents options with a prompt
     */
    class OptionNode : public EntityConversation::Node {
        std::string prompt;
        std::list<std::pair<std::string,std::string> > options;

    public:
        /**
         * Constructs the node with data from the input file
         */
        OptionNode(File& input);

        /**
         * Destructor
         */
        virtual ~OptionNode() = default;

        /**
         * Outputs the text to the given bubble
         */
        virtual void apply(EntityBubble& playerbubble, EntityBubble& owner) override;

        /**
         * Returns a pointer to the next node, or nullptr if the node is not finished
         */
        virtual Node* getNext() override;
    };

    /**
     * Does nothing, provides next node to go to via jump
     */
    class JumpNode : public EntityConversation::Node {
        std::string nextNode;

    public:
        /**
         * Constructs the node with data from the input file
         */
        JumpNode(File& input);

        /**
         * Destructor
         */
        virtual ~JumpNode() = default;

        /**
         * Outputs the text to the given bubble
         */
        virtual void apply(EntityBubble& playerbubble, EntityBubble& owner) override;

        /**
         * Returns a pointer to the next node, or nullptr if the node is not finished
         */
        virtual Node* getNext() override;
    };

    /**
     * Runs a script and jumps based on return value
     */
    class ScriptNode : public EntityConversation::Node {
        std::string script;
        std::string truthyNode, falsyNode;

    public:
        /**
         * Constructs the node with data from the input file
         */
        ScriptNode(File& input);

        /**
         * Destructor
         */
        virtual ~ScriptNode() = default;

        /**
         * Outputs the text to the given bubble
         */
        virtual void apply(EntityBubble& playerbubble, EntityBubble& owner) override;

        /**
         * Returns a pointer to the next node, or nullptr if the node is not finished
         */
        virtual Node* getNext() override;
    };

    /**
     * If this node is current the conversation is over
     */
    class ExitNode : public EntityConversation::Node {
    public:
        /**
         * Creates the ExitNode
         */
        ExitNode() : EntityConversation::Node("EXIT") {}

        /**
         * Destructor
         */
        virtual ~ExitNode() = default;

        /**
         * Outputs the text to the given bubble
         */
        virtual void apply(EntityBubble& playerbubble, EntityBubble& owner) override {}

        /**
         * Returns a pointer to the next node, or nullptr if the node is not finished
         */
        virtual Node* getNext() override { return this; }
    };
};

#endif // ENTITYCONVERSATIONFACTORY_HPP
