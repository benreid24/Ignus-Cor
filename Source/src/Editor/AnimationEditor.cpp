#include "Editor/AnimationEditor.hpp"
#include "Editor/Helpers/Dialogs.hpp"
#include "Shared/Properties.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace sfg;
using namespace sf;

AnimationEditor::AnimationEditor(sfg::Desktop& dk, sfg::Notebook::Ptr parent)
: desktop(dk), owner(parent) {
    AnimationEditor* me = this;

    container = Box::Create(Box::Orientation::VERTICAL,2);
	container->SetRequisition(Vector2f(Properties::ScreenWidth-50,Properties::ScreenHeight-50));

	Button::Ptr editBut = Button::Create("Open Editor");
    editBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->openEditor(); });
    Button::Ptr refreshButton = Button::Create("Refresh Files");
    refreshButton->GetSignal(Button::OnLeftClick).Connect( [me] { me->refresh(); });
    Button::Ptr previewBut = Button::Create("Preview");
    previewBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->preview(); });
    Button::Ptr delBut = Button::Create("Delete");
    delBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->delAnim(); });

    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    butBox->Pack(editBut,false,false);
    butBox->Pack(previewBut,false,false);
    butBox->Pack(delBut,false,false);
    butBox->Pack(refreshButton,false,false);
    container->Pack(butBox,false,false);

    fileList = new FilePicker(desktop, container, "Animation", Properties::AnimationPath, "anim", true);
    refreshOnUpdate = false;
    delOnUpdate = false;

    owner->AppendPage(container,Label::Create("Animations"));
}

AnimationEditor::~AnimationEditor() {
    delete fileList;
}

void AnimationEditor::update() {
    fileList->update(refreshOnUpdate);
    refreshOnUpdate = false;
    if (delOnUpdate) {
        delOnUpdate = false;
        string file = Properties::AnimationPath + fileList->getChoice();
        if (file != Properties::AnimationPath) {
            if (yesnobox(desktop, owner, "Delete Animation?", "Are you sure you want to delete "+fileList->getChoice()+"?")) {
                remove(file.c_str());
                fileList->clear();
                refresh();
            }
        }
        else
            showmessage(desktop, owner, "Error", "No animation selected");
    }
}

void AnimationEditor::openEditor() {
    system("cd tools\\AnimationEditor && AnimationEditor.exe");
}

void AnimationEditor::delAnim() {
    delOnUpdate = true;
}

void AnimationEditor::preview() {
    string file = fileList->getChoice();
    if (file.size() > 0) {
        stringstream cmd;
        cmd << "cd tools\\AnimationEditor && Previewer.exe "
            << Properties::AnimationPath << file << " ..\\..\\";
        system(cmd.str().c_str());
    }
    else
        showmessage(desktop, owner, "Error", "No animation selected");
}

void AnimationEditor::refresh() {
    refreshOnUpdate = true;
}
