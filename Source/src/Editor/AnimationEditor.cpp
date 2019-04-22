#include "Editor/AnimationEditor.hpp"
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
    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    butBox->Pack(editBut,false,false);
    butBox->Pack(refreshButton,false,false);
    container->Pack(butBox,false,false);

    fileList = new FilePicker(desktop, container, "Animation", Properties::AnimationPath, "anim", true);
    refreshOnUpdate = false;

    owner->AppendPage(container,Label::Create("Animations"));
}

AnimationEditor::~AnimationEditor() {
    delete fileList;
}

void AnimationEditor::update() {
    fileList->update(refreshOnUpdate);
    refreshOnUpdate = false;
}

void AnimationEditor::openEditor() {
    system("cd tools\\AnimationEditor && AnimationEditor.exe");
}

void AnimationEditor::refresh() {
    refreshOnUpdate = true;
}
