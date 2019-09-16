#include "Editor/ConversationEditor.hpp"
#include "Editor/Helpers/Dialogs.hpp"
#include "Editor/Helpers/ConversationEditorWindow.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Util/File.hpp"
using namespace std;
using namespace sfg;

ConversationEditor::ConversationEditor(Desktop& dk, Notebook::Ptr o)
: desktop(dk), owner(o)
, container(Box::Create(Box::Orientation::VERTICAL))
, newConv(false), editConv(false), delConv(false) {
    Box::Ptr box = Box::Create();

    Button::Ptr button = Button::Create("New");
    button->GetSignal(Button::OnLeftClick).Connect([this] { this->newConv = true; });
    box->Pack(button,false,false);

    button = Button::Create("Edit");
    button->GetSignal(Button::OnLeftClick).Connect([this] { this->editConv = true; });
    box->Pack(button,false,false);

    button = Button::Create("Delete");
    button->GetSignal(Button::OnLeftClick).Connect([this] { this->delConv = true; });
    box->Pack(button,false,false);

    container->Pack(box, false, false);
    picker = new FilePicker(dk, container, "Conversations", Properties::ConversationPath, "conv", true);
    owner->AppendPage(container, Label::Create("Conversation Editor"));
}

void ConversationEditor::update() {
    picker->update(false);

    if (newConv) {
        newConv = false;
        ConversationEditorWindow window(desktop, owner, "new conversation");
        window.editConversation();
        picker->update(true);
    }
    if (editConv) {
        editConv = false;
        ConversationEditorWindow window(desktop, owner, picker->getChoice());
        window.editConversation();
        picker->update(true);
    }
    if (delConv) {
        delConv = false;
        if (File::exists(Properties::ConversationPath+picker->getChoice())) {
            if (yesnobox(desktop, owner, "Delete Conversation", "Are you sure you want to delete "+picker->getChoice()+"?")) {
                remove(string(Properties::ConversationPath+picker->getChoice()).c_str());
                picker->update(true);
            }
        }
    }
}
