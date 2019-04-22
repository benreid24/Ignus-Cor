#include "Editor/AnimationEditor.hpp"
#include "Editor/Helpers/FilePicker.hpp"
#include "Shared/Properties.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace sfg;
using namespace sf;

AnimationEditor::AnimationEditor(sfg::Desktop& dk, sfg::Notebook::Ptr parent) : desktop(dk), owner(parent) {
    AnimationEditor* me = this;

    container = Box::Create(Box::Orientation::VERTICAL,2);
	container->SetRequisition(Vector2f(Properties::ScreenWidth-50,Properties::ScreenHeight-50));

	Button::Ptr fileBut = Button::Create("View Files"), editBut = Button::Create("Open Editor");
    fileBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->showFiles(); });
    editBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->openEditor(); });
    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    butBox->Pack(fileBut,false,false);
    butBox->Pack(editBut,false,false);
    container->Pack(butBox,false,false);

    owner->AppendPage(container,Label::Create("Animations"));
}

void AnimationEditor::update() {
    //update
}

void AnimationEditor::showFiles() {
    FilePicker picker(desktop, owner, "Animations", Properties::AnimationPath, ".anim");
    picker.pickFile();
}

void AnimationEditor::openEditor() {
    system("cd tools\\AnimationEditor && AnimationEditor.exe");
}
