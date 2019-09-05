#include <Shared/Entities/EntityConversationNodeFactory.hpp>
#include <Shared/Scripts/ScriptManager.hpp>
using namespace std;

list<EntityConversation::Node::Ptr> EntityConversationNodeFactory::load(File& input) {
    list<EntityConversation::Node::Ptr> nodes;
    size_t nodeCount = input.get<uint16_t>();

    for (unsigned int i = 0; i<nodeCount; ++i) {
        char t = input.get<char>();
        switch (t) {

        case 't':
            nodes.emplace_back(new TalkNode(input));
            break;

        case 'o':
            nodes.emplace_back(new OptionNode(input));
            break;

        case 'j':
            nodes.emplace_back(new JumpNode(input));
            break;

        case 's':
            nodes.emplace_back(new ScriptNode(input));
            break;

        case 'e':
            nodes.emplace_back(new ExitNode());
            break;

        default:
            cout << "Error: Invalid conversation node type '" << t << "', terminating load\n";
            return nodes;
        }
    }

    return nodes;
}

EntityConversation::Node::Ptr EntityConversationNodeFactory::getExitNode() {
    return EntityConversation::Node::Ptr(new ExitNode());
}

EntityConversationNodeFactory::TalkNode::TalkNode(File& input)
: EntityConversation::Node(input), bubbleId(-1), done(false) {
    say = input.getString();
    nextNode = input.getString();
}

void EntityConversationNodeFactory::TalkNode::apply(EntityConversation* conv, EntityBubble& playerbubble, EntityBubble& owner) {
    if (bubbleId < 0)
        bubbleId = owner.addContent(say);
    else
        done = owner.queryContent(bubbleId) == EntityBubble::Finished;
}

EntityConversation::Node* EntityConversationNodeFactory::TalkNode::getNext(EntityConversation* conv) {
    return done ? conv->getNode(nextNode) : nullptr;
}

EntityConversationNodeFactory::JumpNode::JumpNode(File& input)
: EntityConversation::Node(input) {
    nextNode = input.getString();
}

EntityConversation::Node* EntityConversationNodeFactory::JumpNode::getNext(EntityConversation* conv) {
    return conv->getNode(nextNode);
}

EntityConversationNodeFactory::OptionNode::OptionNode(File& input)
: EntityConversation::Node(input), promptId(-1), optionsId(-1), chosenOption(-1) {
    prompt = input.getString();
    size_t nopts = input.get<uint16_t>();
    options.reserve(nopts);
    for (unsigned int i = 0; i<nopts; ++i) {
        string option = input.getString();
        string node = input.getString();
        options.emplace_back(option, node);
    }
}

void EntityConversationNodeFactory::OptionNode::apply(EntityConversation* conv, EntityBubble& playerbubble, EntityBubble& owner) {
    if (promptId < 0)
        promptId = owner.addContent(prompt, -1);
    if (optionsId < 0)
        optionsId = playerbubble.addContent("TODO", -1);
    chosenOption = playerbubble.queryContentSpecificStatus(optionsId);
    if (chosenOption >= 0) {
        owner.removeContent(promptId);
        playerbubble.removeContent(optionsId);
    }
}

EntityConversation::Node* EntityConversationNodeFactory::OptionNode::getNext(EntityConversation* conv) {
    return chosenOption>=0 ? conv->getNode(options[chosenOption].second) : nullptr;
}

EntityConversationNodeFactory::ScriptNode::ScriptNode(File& input)
: EntityConversation::Node(input), started(false) {
    script.reset(new Script(input.getString()));
    truthyNode = input.getString();
    falsyNode = input.getString();
}

void EntityConversationNodeFactory::ScriptNode::apply(EntityConversation* conv, EntityBubble& playerbubble, EntityBubble& owner) {
    if (!started) {
        ScriptManager::get().runScript(script, true);
        started = true;
    }
}

EntityConversation::Node* EntityConversationNodeFactory::ScriptNode::getNext(EntityConversation* conv) {
    return (started && !script->isRunning()) ? conv->getNode(truthyNode) : nullptr; //TODO - get script result
}
