#include "Editor/ScriptEditor.hpp"
#include "Editor/Helpers/Dialogs.hpp"
#include "Editor/Helpers/ScriptEditorWindow.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Util/File.hpp"
using namespace std;
using namespace sfg;

ScriptEditor::ScriptEditor(Desktop& dk, Notebook::Ptr o)
: desktop(dk), owner(o)
, container(Box::Create(Box::Orientation::VERTICAL))
, newScript(false), editScript(false), delScript(false) {
    Box::Ptr box = Box::Create();

    Button::Ptr button = Button::Create("New");
    button->GetSignal(Button::OnLeftClick).Connect([this] { this->newScript = true; });
    box->Pack(button,false,false);

    button = Button::Create("Edit");
    button->GetSignal(Button::OnLeftClick).Connect([this] { this->editScript = true; });
    box->Pack(button,false,false);

    button = Button::Create("Delete");
    button->GetSignal(Button::OnLeftClick).Connect([this] { this->delScript = true; });
    box->Pack(button,false,false);

    container->Pack(box, false, false);
    picker = new FilePicker(dk, container, "Scripts", Properties::ScriptPath, Properties::ScriptExtension, true);
    owner->AppendPage(container, Label::Create("Script Editor"));
}

void ScriptEditor::update() {
    picker->update(false);

    if (newScript) {
        newScript = false;
        ScriptEditorWindow window(desktop, owner, "");
        window.getScript();
        picker->update(true);
    }
    if (editScript) {
        editScript = false;
        ScriptEditorWindow window(desktop, owner, picker->getChoice());
        window.getScript();
        picker->update(true);
    }
    if (delScript) {
        delScript = false;
        if (File::exists(Properties::ScriptPath+picker->getChoice())) {
            if (yesnobox(desktop, owner, "Delete Script", "Are you sure you want to delete "+picker->getChoice()+"?")) {
                remove(string(Properties::ScriptPath+picker->getChoice()).c_str());
                picker->update(true);
            }
        }
    }
}
