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
    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    butBox->Pack(editBut,false,false);
    container->Pack(butBox,false,false);

    fileList = new FilePicker(desktop, container, "Animations", Properties::AnimationPath, "anim", true);

    owner->AppendPage(container,Label::Create("Animations"));
}

void AnimationEditor::update() {
    fileList->update();
}

void AnimationEditor::openEditor() {
    system("cd tools\\AnimationEditor && AnimationEditor.exe");
}
