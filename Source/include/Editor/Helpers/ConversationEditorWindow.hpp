#ifndef CONVERSATIONEDITORWINDOW_HPP
#define CONVERSATIONEDITORWINDOW_HPP

#include "Editor/GUI/Form.hpp"
#include "Shared/Entities/EntityConversationNodeFactory.hpp"
#include <SFGUI/Widgets.hpp>

/**
 * Class that creates a window for creating or editing conversations
 *
 * \ingroup Editor
 */
class ConversationEditorWindow {
    /**
     * Helper struct to store data for conversations
     */
    struct EditorConvNode {
        std::string name;
        std::string data; //say, next node, script, option prompt
        std::string trueNode, falseNode; //script only
        std::vector<std::pair<std::string,std::string> > choices; //option only

        enum Type: char {
            Talk = 't',
            Option = 'o',
            Jump = 'j',
            Script = 's',
            Exit = 'e'
        }type;

        static Type fromIndex(int i) {
            switch (i) {
            case 0:
                return Talk;
            case 1:
                return Option;
            case 2:
                return Jump;
            case 3:
                return Script;
            case 4:
                return Exit;
            default:
                return Talk;
            }
        }

        static int fromType(Type t) {
            switch (t) {
            case Talk:
                return 0;
            case Option:
                return 1;
            case Jump:
                return 2;
            case Script:
                return 3;
            case Exit:
                return 4;
            default:
                return 0;
            }
        }

        static const std::vector<std::string> typeStrings;
    };

public:
    /**
     * Loads an existing conversation from file and edits it. If the file does not exist, creates a new conversation
     */
    ConversationEditorWindow(sfg::Widget::Ptr parent, const std::string& file);

    /**
     * Creates the window and performs the actual editing. Returns the filename of the created conversation
     */
    std::string editConversation(sfg::Desktop& desktop);

private:
    std::string filename;
    std::vector<EditorConvNode> nodes;
    std::string currentNode;
    bool dirty, needsRefresh;
    bool closed, nodeDeleted;

    sfg::Widget::Ptr parent;
    sfg::Window::Ptr window;
    sfg::Entry::Ptr fileEntry;

    sfg::ScrolledWindow::Ptr nodeArea;
    sfg::Box::Ptr nodeBox;

    sfg::ScrolledWindow::Ptr invalidNodeArea;
    sfg::Box::Ptr invalidNodeBox;
    std::vector<std::string> invalidNodes;

    sfg::ScrolledWindow::Ptr unreachableNodeArea;
    sfg::Box::Ptr unreachableNodeBox;
    std::vector<std::string> unreachableNodes;

    sfg::Box::Ptr sourceNodesBox, destNodesBox, nodePropsBox;
    Form nodeForm;

    /**
     * Returns the index of the node with the given name. Creates it first if it doesn't exist
     */
    unsigned int getNodeIndex(const std::string& name);

    /**
     * Returns true if the given node exists or not
     */
    bool nodeExists(const std::string& name);

    /**
     * Returns whether or not the given node is valid
     */
    bool isNodeValid(const std::string& name);

    /**
     * Returns the list of nodes referencing the given node
     */
    std::vector<std::string> getReferencingNodes(const std::string& name);

    /**
     * Returns the list of nodes referenced by the given node
     */
    std::vector<std::string> getNodesReferenced(const std::string& name);

    /**
     * Updates the list of invalid nodes and unreachable nodes
     */
    void recalcInvalidNodes();

    /**
     * Repopulates all GUI elements that are subject to change
     * Fills lists, populates forms, updates node references
     */
    void refreshGui();

    /**
     * Changes the currently selected node and refreshes the GUI
     */
    void selectNode(const std::string& name);

    /**
     * Inserts a new node before the currently selected node
     */
    void insertNode();

    /**
     * Appends a new node after the currently selected node
     */
    void appendNode();

    /**
     * Deletes the selected node
     */
    void deleteNode();

    /**
     * Moves the selected node up
     */
    void moveNodeUp();

    /**
     * Moves the selected node down
     */
    void moveNodeDown();

    /**
     * Updates the current node from the properties input form
     */
    void updateNodeProps();

    /**
     * Callback for when new node type is picked
     */
    void nodeTypeChangeCb(int type);

    /**
     * Saves the conversation to the file. Creates subfolders if need be
     */
    void save();

    /**
     * Closes the conversation. Prompts to save if not saved
     */
    void close();
};

#endif // CONVERSATIONEDITORWINDOW_HPP
