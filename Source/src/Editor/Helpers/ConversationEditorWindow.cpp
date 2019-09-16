#include "Editor/Helpers/ConversationEditorWindow.hpp"
#include "Editor/Helpers/Dialogs.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Properties.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFGUI/RadioButtonGroup.hpp>
#include <SFGUI/SFGUI.hpp>
using namespace std;
using namespace sfg;

extern sf::RenderWindow sfWindow;
extern SFGUI sfgui;

const vector<string> ConversationEditorWindow::EditorConvNode::typeStrings = {"Talk", "Options", "Jump", "Script", "Exit"};

ConversationEditorWindow::ConversationEditorWindow(Desktop& dk, Widget::Ptr pr, const string& file)
: desktop(dk), parent(pr), dirty(false), needsRefresh(false), closed(false)
, nodeDeleted(false), saved(false), window(Window::Create())
, nodeOptions(dk, window, vector<string>({"Choice", "Node"}), "Option") {
    if (File::exists(Properties::ConversationPath+file)) {
        filename = file;
        File input(Properties::ConversationPath+file);
        EditorConvNode node;
        unsigned int nodeCount = input.get<uint16_t>();
        for (unsigned int i = 0; i<nodeCount; ++i) {
            node.type = EditorConvNode::Type(input.get<char>());
            node.name = input.getString();
            if (node.type == EditorConvNode::Talk)
                node.data = input.getString();
            else if (node.type == EditorConvNode::Option) {
                node.data = input.getString();
                unsigned int nopts = input.get<uint16_t>();
                for (unsigned int j = 0; j<nopts; ++j) {
                    string first = input.getString();
                    string second = input.getString();
                    node.choices.emplace_back(first, second);
                }
            }
            else if (node.type == EditorConvNode::Jump)
                node.data = input.getString();
            else if (node.type == EditorConvNode::Script) {
                node.data = input.getString();
                node.trueNode = input.getString();
                node.falseNode = input.getString();
            }
            else if (node.type != EditorConvNode::Exit)
                cout << "Error: Unsupported node type " << char(node.type) << endl;
            nodes.push_back(node);
        }
    }
    else if (File::getExtension(file) != "conv")
        filename = file + ".conv";
    else
        filename = file;

    if (nodes.size() == 0) {
        EditorConvNode node;
        node.type = EditorConvNode::Talk;
        node.name = "start";
        nodes.push_back(node);
        node.type = EditorConvNode::Exit;
        node.name = "end";
        nodes.push_back(node);
    }

    //set up GUI
    ConversationEditorWindow* me = this;
    window->SetTitle("Conversation Editor");

    Box::Ptr winBox = Box::Create(Box::Orientation::VERTICAL, 15);
    Box::Ptr topArea = Box::Create(Box::Orientation::HORIZONTAL, 10);
    Box::Ptr bottomArea = Box::Create(Box::Orientation::HORIZONTAL, 10);
    winBox->Pack(topArea);
    winBox->Pack(Separator::Create(Separator::Orientation::HORIZONTAL), false, false);
    winBox->Pack(bottomArea);
    window->Add(winBox);

    Box::Ptr vertBox = Box::Create(Box::Orientation::VERTICAL, 5);
    Box::Ptr horBox = Box::Create(Box::Orientation::HORIZONTAL, 5);

    //Control area (file, save, close)
    vertBox->SetRequisition(sf::Vector2f(200,300));
    fileEntry = Entry::Create(filename);
    vertBox->Pack(Label::Create("Filename: "), false, false);
    vertBox->Pack(fileEntry);
    horBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    Button::Ptr button = Button::Create("Save");
    button->GetSignal(Button::OnLeftClick).Connect( [me] { me->save(); });
    horBox->Pack(button, false, false);
    button = Button::Create("Close");
    button->GetSignal(Button::OnLeftClick).Connect( [me] { me->close(); });
    horBox->Pack(button, false, false);
    vertBox->Pack(horBox, false, false);
    topArea->Pack(vertBox);
    topArea->Pack(Separator::Create(Separator::Orientation::VERTICAL), false, false);

    //Node create/delete area
    vertBox = Box::Create(Box::Orientation::VERTICAL, 10);
    vertBox->SetRequisition(sf::Vector2f(200,300));
    button = Button::Create("Insert Before Selected");
    button->GetSignal(Button::OnLeftClick).Connect( [me] { me->insertNode(); });
    vertBox->Pack(button, false, false);
    button = Button::Create("Insert After Selected");
    button->GetSignal(Button::OnLeftClick).Connect( [me] { me->appendNode(); });
    vertBox->Pack(button, false, false);
    button = Button::Create("Delete Selected");
    button->GetSignal(Button::OnLeftClick).Connect( [me] { me->deleteNode(); });
    vertBox->Pack(button, false, false);
    topArea->Pack(vertBox);

    //node list area
    nodeArea = ScrolledWindow::Create();
    nodeArea->SetRequisition(sf::Vector2f(200,300));
    nodeArea->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_ALWAYS );
    nodeBox = Box::Create(Box::Orientation::VERTICAL, 5);
    nodeBox->SetRequisition(sf::Vector2f(200,300));
    nodeArea->AddWithViewport(nodeBox);
    topArea->Pack(nodeArea);

    //node re-arrange area
    vertBox = Box::Create(Box::Orientation::VERTICAL, 10);
    vertBox->SetRequisition(sf::Vector2f(200,300));
    button = Button::Create("Move up");
    button->GetSignal(Button::OnLeftClick).Connect( [me] { me->moveNodeUp(); });
    vertBox->Pack(button, false, false);
    button = Button::Create("Move down");
    button->GetSignal(Button::OnLeftClick).Connect( [me] { me->moveNodeDown(); });
    vertBox->Pack(button, false, false);
    topArea->Pack(vertBox);
    topArea->Pack(Separator::Create(Separator::Orientation::VERTICAL), false, false);

    //invalid node list area
    vertBox = Box::Create(Box::Orientation::VERTICAL);
    invalidNodeArea = ScrolledWindow::Create();
    invalidNodeArea->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_ALWAYS );
    invalidNodeArea->SetRequisition(sf::Vector2f(200,300));
    invalidNodeBox = Box::Create(Box::Orientation::VERTICAL);
    invalidNodeBox->SetRequisition(sf::Vector2f(200,300));
    invalidNodeArea->AddWithViewport(invalidNodeBox);
    vertBox->Pack(Label::Create("Invalid nodes:"), false, false);
    vertBox->Pack(invalidNodeArea, false, false);
    topArea->Pack(vertBox);
    topArea->Pack(Separator::Create(Separator::Orientation::VERTICAL), false, false);

    //unreachable node area
    vertBox = Box::Create(Box::Orientation::VERTICAL);
    unreachableNodeArea = ScrolledWindow::Create();
    unreachableNodeArea->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_ALWAYS );
    unreachableNodeArea->SetRequisition(sf::Vector2f(200,300));
    unreachableNodeBox = Box::Create(Box::Orientation::VERTICAL);
    unreachableNodeBox->SetRequisition(sf::Vector2f(200,300));
    unreachableNodeArea->AddWithViewport(unreachableNodeBox);
    vertBox->Pack(Label::Create("Unreachable nodes"), false, false);
    vertBox->Pack(unreachableNodeArea, false, false);
    topArea->Pack(vertBox);

    //referenced by area
    sourceNodesBox = Box::Create(Box::Orientation::VERTICAL, 5);
    sourceNodesBox->SetRequisition(sf::Vector2f(200,300));
    bottomArea->Pack(sourceNodesBox);
    bottomArea->Pack(Separator::Create(Separator::Orientation::VERTICAL), false, false);

    //node properties area
    nodePropsBox = Box::Create(Box::Orientation::VERTICAL, 5);
    nodePropsBox->SetRequisition(sf::Vector2f(200,300));
    nodeForm.addToParent(nodePropsBox);
    nodeOptions.addToParent(nodePropsBox);
    nodeForm.addField("name", "Name: ", 160);
    nodeForm.addDropdown("type", "Type: ",
                         EditorConvNode::typeStrings,
                         [me] (int t) { me->nodeTypeChangeCb(t); });
    nodeForm.addField("data", "Data: ", 300);
    button = Button::Create("Update Node");
    button->GetSignal(Button::OnLeftClick).Connect( [me] { me->updateNodeProps(); });
    nodePropsBox->Pack(button);
    bottomArea->Pack(nodePropsBox);
    bottomArea->Pack(Separator::Create(Separator::Orientation::VERTICAL), false, false);

    //references area
    destNodesBox = Box::Create(Box::Orientation::VERTICAL, 5);
    destNodesBox->SetRequisition(sf::Vector2f(200,300));
    bottomArea->Pack(destNodesBox);

    currentNode = nodes[0].name;
    refreshGui();
}

string ConversationEditorWindow::editConversation() {
    parent->SetState(Widget::State::INSENSITIVE);
    desktop.Add(window);

    while (sfWindow.isOpen()) {
        sf::Event event;
        while (sfWindow.pollEvent(event)) {
            desktop.HandleEvent(event);

            if (event.type==sf::Event::Closed)
                sfWindow.close();
        }
        desktop.Update(30/1000);
        nodeForm.update();
        nodeOptions.update();

        if (closed) {
            closed = false;
            if (dirty) {
                if (yesnobox(desktop, window, "Close without saving?", "Unsaved changes! Close anyway?"))
                    break;
            }
            else
                break;
        }
        if (nodeDeleted) {
            nodeDeleted = false;
            if (nodes.size() > 1) {
                if (yesnobox(desktop, window, "Delete node?", "Are you sure you want to delete "+currentNode+"?")) {
                    unsigned int i = getNodeIndex(currentNode);
                    nodes.erase(nodes.begin()+i);
                    if (i >= nodes.size())
                        i = nodes.size()-1;
                    currentNode = nodes[i].name;
                    refreshGui();
                }
            }
            else
                showmessage(desktop, window, "Error", "Need at least 1 node");
        }
        if (needsRefresh) {
            needsRefresh = false;
            refreshGui();
        }
        if (saved) {
            saved = false;
            bool performSave = true;
            if (invalidNodes.size() > 0 || unreachableNodes.size() > 0) {
                performSave = yesnobox(desktop, window, "Save with errors?", "Errors present, save anyways?");
            }
            if (performSave)
                doSave();
        }

        desktop.BringToFront(window);
        sfWindow.clear();
        sfgui.Display(sfWindow);
        sfWindow.display();
        sf::sleep(sf::milliseconds(30));
    }

    desktop.Remove(window);
    parent->SetState(Widget::State::NORMAL);

    return fileEntry->GetText();
}

unsigned int ConversationEditorWindow::getNodeIndex(const string& name) {
    for (unsigned int i = 0; i<nodes.size(); ++i) {
        if (nodes[i].name == name)
            return i;
    }
    EditorConvNode node;
    node.type = EditorConvNode::Talk;
    node.name = name;
    nodes.push_back(node);
    return nodes.size()-1;
}

bool ConversationEditorWindow::nodeExists(const string& name) {
    for (unsigned int i = 0; i<nodes.size(); ++i) {
        if (nodes[i].name == name)
            return true;
    }
    return false;
}

vector<string> ConversationEditorWindow::getReferencingNodes(const string& name) {
    vector<string> callers;

    if (!nodeExists(name))
        return callers;

    unsigned int ni = getNodeIndex(name);
    if (ni >= 1) {
        if (nodes[ni-1].type == EditorConvNode::Talk)
            callers.push_back(nodes[ni-1].name);
    }

    for (unsigned int i = 0; i<nodes.size(); ++i) {
        switch (nodes[i].type) {
        case EditorConvNode::Option: {
                for (unsigned int j = 0; j<nodes[i].choices.size(); ++j) {
                    if (nodes[i].choices[j].second == name) {
                        callers.push_back(nodes[i].name);
                        break;
                    }
                }
            }
            break;

        case EditorConvNode::Jump:
            if (nodes[i].data == name)
                callers.push_back(nodes[i].name);
            break;

        case EditorConvNode::Script:
            if (nodes[i].trueNode == name || nodes[i].falseNode == name)
                callers.push_back(nodes[i].name);
            break;

        default:
            break;
        }
    }

    return callers;
}

vector<string> ConversationEditorWindow::getNodesReferenced(const string& name) {
    vector<string> called;

    if (!nodeExists(name))
        return called;

    EditorConvNode& node = nodes[getNodeIndex(name)];

    switch (node.type) {
        case EditorConvNode::Talk: {
            unsigned int i = getNodeIndex(name) + 1;
            if (i < nodes.size())
                called.push_back(nodes[i].name);
        }
        break;

        case EditorConvNode::Script:
            called.push_back(node.trueNode);
            called.push_back(node.falseNode);
            break;

        case EditorConvNode::Jump:
            called.push_back(node.data);
            break;

        case EditorConvNode::Option: {
                for (unsigned int i = 0; i<node.choices.size(); ++i) {
                    called.push_back(node.choices[i].second);
                }
            }
            break;

        default:
            break;
    }

    for (unsigned int i = 0; i<called.size(); ++i) {
        if (called[i].empty()) {
            called.erase(called.begin()+i);
            --i;
        }
    }

    return called;
}

bool ConversationEditorWindow::isNodeValid(const string& name) {
    if (!nodeExists(name))
        return false;

    EditorConvNode& node = nodes[getNodeIndex(name)];

    if (node.name.empty())
        return false;

    switch (node.type) {
        case EditorConvNode::Script:
            if (!nodeExists(node.trueNode))
                return false;
            if (!nodeExists(node.falseNode))
                return false;
            break;

        case EditorConvNode::Jump:
            if (!nodeExists(node.data))
                return false;
            break;

        case EditorConvNode::Option: {
                for (unsigned int i = 0; i<node.choices.size(); ++i) {
                    if (!nodeExists(node.choices[i].second))
                        return false;
                }
            }
            break;

        default:
            break;
    }

    return true;
}

void ConversationEditorWindow::recalcInvalidNodes() {
    vector<string> reachableNodes, seenNodes;

    invalidNodes.clear();
    for (unsigned int i = 0; i<nodes.size(); ++i) {
        if (find(seenNodes.begin(), seenNodes.end(), nodes[i].name) != seenNodes.end())
            invalidNodes.push_back(nodes[i].name);
        else
            seenNodes.push_back(nodes[i].name);

        vector<string> calledNodes = getNodesReferenced(nodes[i].name);
        reachableNodes.insert(reachableNodes.end(), calledNodes.begin(), calledNodes.end());
        if (!isNodeValid(nodes[i].name))
            invalidNodes.push_back(nodes[i].name);
    }

    unreachableNodes.clear();
    for (unsigned int i = 0; i<nodes.size(); ++i) {
        if (find(reachableNodes.begin(), reachableNodes.end(), nodes[i].name) == reachableNodes.end() && i!=0)
            unreachableNodes.push_back(nodes[i].name);
    }

    //add nodes only "reachable" from unreachable nodes until unreachable nodes stops growing
    unsigned int size = 0;
    do {
        size = unreachableNodes.size();
        for (unsigned int i = 1; i<nodes.size(); ++i) {
            if (find(unreachableNodes.begin(), unreachableNodes.end(), nodes[i].name) != unreachableNodes.end())
                continue;
            vector<string> sources = getReferencingNodes(nodes[i].name);
            for (unsigned int j = 0; j<sources.size(); ++j) {
                if (find(unreachableNodes.begin(), unreachableNodes.end(), sources[j]) == unreachableNodes.end())
                    goto reachable;
            }
            unreachableNodes.push_back(nodes[i].name);
            reachable:;
        }
    } while (size != unreachableNodes.size());
}

void ConversationEditorWindow::selectNode(const string& name) {
    getNodeIndex(name); //ensure exists
    if (name != currentNode)
        updateNodeProps();
    currentNode = name;
    needsRefresh = true;
}

void ConversationEditorWindow::insertNode() {
    dirty = true;
    updateNodeProps();
    unsigned int i = getNodeIndex(currentNode);
    EditorConvNode node;
    node.name = "set me";
    node.type = EditorConvNode::Talk;
    nodes.insert(nodes.begin()+i, node);
    needsRefresh = true;
}

void ConversationEditorWindow::appendNode() {
    dirty = true;
    updateNodeProps();
    unsigned int i = getNodeIndex(currentNode);
    EditorConvNode node;
    node.name = "set me";
    node.type = EditorConvNode::Talk;
    nodes.insert(nodes.begin()+i+1, node);
    needsRefresh = true;
}

void ConversationEditorWindow::deleteNode() {
    nodeDeleted = true;
}

void ConversationEditorWindow::moveNodeUp() {
    unsigned int i = getNodeIndex(currentNode);
    if (i == 0)
        return;

    updateNodeProps();
    EditorConvNode temp = nodes[i];
    nodes[i] = nodes[i-1];
    nodes[i-1] = temp;
    dirty = true;
    needsRefresh = true;
}

void ConversationEditorWindow::moveNodeDown() {
    unsigned int i = getNodeIndex(currentNode);
    if (i == nodes.size()-1)
        return;

    updateNodeProps();
    EditorConvNode temp = nodes[i];
    nodes[i] = nodes[i+1];
    nodes[i+1] = temp;
    dirty = true;
    needsRefresh = true;
}

void ConversationEditorWindow::updateNodeProps() {
    unsigned int i = getNodeIndex(currentNode);

    nodes[i].name = nodeForm.getField("name");
    for (unsigned int j = 0; j<nodes.size(); ++j) {
        if (j != i) {
            switch (nodes[j].type) {
            case EditorConvNode::Option: {
                    for (unsigned int k = 0; k<nodes[j].choices.size(); ++k) {
                        if (nodes[j].choices[k].second==currentNode)
                            nodes[j].choices[k].second = nodes[i].name;
                    }
                }
                break;
            case EditorConvNode::Jump:
                if (nodes[j].data == currentNode)
                    nodes[j].data = nodes[i].name;
                break;
            case EditorConvNode::Script:
                if (nodes[j].trueNode == currentNode)
                    nodes[j].trueNode = nodes[i].name;
                if (nodes[j].falseNode == currentNode)
                    nodes[j].falseNode = nodes[i].name;
                break;
            default:
                break;
            }
        }
    }
    currentNode = nodes[i].name;

    nodes[i].data = nodeForm.getField("data");
    int t = nodeForm.getSelectedDropdownOption("type");
    nodes[i].type = EditorConvNode::fromIndex(t);
    if (nodes[i].type == EditorConvNode::Option) {
        vector<vector<string> > options = nodeOptions.getAllValues();
        nodes[i].choices.clear();
        for (unsigned int j = 0; j<options.size(); ++j) {
            nodes[i].choices.emplace_back(options[j][0], options[j][1]);
        }
    }

    dirty = true;
    needsRefresh = true;
}

void ConversationEditorWindow::nodeTypeChangeCb(int type) {
    updateNodeProps();
    needsRefresh = true;
}

void ConversationEditorWindow::save() {
    saved = true;
}

void ConversationEditorWindow::close() {
    closed = true;
}

void ConversationEditorWindow::refreshGui() {
    ConversationEditorWindow* me = this;

    recalcInvalidNodes();
    vector<string> callers = getReferencingNodes(currentNode);
    vector<string> called = getNodesReferenced(currentNode);

    invalidNodeBox->RemoveAll();
    for (unsigned int i = 0; i<invalidNodes.size(); ++i) {
        string nm = invalidNodes[i];
        Button::Ptr but = Button::Create(nm);
        but->GetSignal(Button::OnLeftClick).Connect( [me, nm] { me->selectNode(nm); });
        invalidNodeBox->Pack(but);
    }
    if (invalidNodes.size() == 0)
        invalidNodeBox->Pack(Label::Create("None"));

    unreachableNodeBox->RemoveAll();
    for (unsigned int i = 0; i<unreachableNodes.size(); ++i) {
        string nm = unreachableNodes[i];
        Button::Ptr but = Button::Create(nm);
        but->GetSignal(Button::OnLeftClick).Connect( [me, nm] { me->selectNode(nm); });
        unreachableNodeBox->Pack(but);
    }
    if (unreachableNodes.size() == 0)
        unreachableNodeBox->Pack(Label::Create("None"));

    nodeBox->RemoveAll();
    RadioButtonGroup::Ptr group = RadioButtonGroup::Create();
    for (unsigned int i = 0; i<nodes.size(); ++i) {
        string nm = nodes[i].name;
        RadioButton::Ptr but = RadioButton::Create(nm);
        but->GetSignal(RadioButton::OnLeftClick).Connect( [me, nm] { me->selectNode(nm); });
        if (currentNode == nm)
            but->SetActive(true);
        nodeBox->Pack(but);
    }

    sourceNodesBox->RemoveAll();
    sourceNodesBox->Pack(Label::Create("Nodes Referencing this node:"), false, false);
    for (unsigned int i = 0; i<callers.size(); ++i) {
        string nm = callers[i];
        Button::Ptr but = Button::Create(nm);
        but->GetSignal(Button::OnLeftClick).Connect( [me, nm] { me->selectNode(nm); });
        sourceNodesBox->Pack(but);
    }
    if (callers.size() == 0)
        sourceNodesBox->Pack(Label::Create("None"));

    destNodesBox->RemoveAll();
    destNodesBox->Pack(Label::Create("Nodes Referenced by this node:"), false, false);
    for (unsigned int i = 0; i<called.size(); ++i) {
        string nm = called[i];
        Button::Ptr but = Button::Create(nm);
        but->GetSignal(Button::OnLeftClick).Connect( [me, nm] { me->selectNode(nm); });
        if (!nodeExists(nm))
            but->SetLabel("(DOESN'T EXIST) "+nm);
        destNodesBox->Pack(but);
    }
    if (called.size() == 0)
        destNodesBox->Pack(Label::Create("None"));

    EditorConvNode& node = nodes[getNodeIndex(currentNode)];
    nodeForm.setField("name", node.name);
    nodeForm.setField("data", node.data);
    nodeForm.setDropdownSelection("type", EditorConvNode::fromType(node.type));

    nodeForm.showInput("data");
    nodeOptions.show(false);
    switch (node.type) {
    case EditorConvNode::Talk:
        nodeForm.updateFieldLabel("data", "Line: ");
        break;
    case EditorConvNode::Jump:
        nodeForm.updateFieldLabel("data", "Node: ");
        break;

    case EditorConvNode::Script:
        nodeForm.updateFieldLabel("data", "Script: "); //TODO - script entry window w/ syntax checking
        break;

    case EditorConvNode::Option: {
        nodeForm.updateFieldLabel("data", "Prompt: ");
        nodeOptions.show(true);
        nodeOptions.clear();
        for (unsigned int i = 0; i<node.choices.size(); ++i) {
            nodeOptions.appendRow(vector<string>({node.choices[i].first, node.choices[i].second}));
        }
        }
        break;
    default:
        nodeForm.hideInput("data");
    }
}

void ConversationEditorWindow::doSave() {
    dirty = false;

    string saveFile = fileEntry->GetText();
    if (File::getExtension(saveFile) != "conv") {
        saveFile += ".conv";
        fileEntry->SetText(saveFile);
    }
    File::createDirectories(Properties::ConversationPath+File::getPath(saveFile));

    File output(Properties::ConversationPath+saveFile, File::Out);

    output.write<uint16_t>(nodes.size());
    for (unsigned int i = 0; i<nodes.size(); ++i) {
        output.write<char>(char(nodes[i].type));
        output.writeString(nodes[i].name);

        if (nodes[i].type != EditorConvNode::Exit)
            output.writeString(nodes[i].data);

        if (nodes[i].type == EditorConvNode::Option) {
            output.write<uint16_t>(nodes[i].choices.size());
            for (unsigned int j = 0; j<nodes[i].choices.size(); ++j) {
                output.writeString(nodes[i].choices[j].first);
                output.writeString(nodes[i].choices[j].second);
            }
        }
        else if (nodes[i].type == EditorConvNode::Script) {
            output.writeString(nodes[i].trueNode);
            output.writeString(nodes[i].falseNode);
        }
    }
}
