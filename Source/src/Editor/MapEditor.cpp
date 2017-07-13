#include <iostream>
#include "Editor/MapEditor.hpp"
using namespace sfg;
using namespace sf;
using namespace std;

MapEditor::MapEditor(Desktop& desktop, Notebook::Ptr parent) : tileBox(Box::Orientation::HORIZONTAL,5,6), animBox(Box::Orientation::HORIZONTAL,5,6) {
	MapEditor* me = this;

	container = Box::Create();
	tabs = Notebook::Create();
	tabs->SetRequisition(Vector2f(300,860));
	mapArea = Canvas::Create();
	mapArea->SetRequisition(Vector2f(1300,860));
	container->Pack(tabs);
	container->Pack(mapArea);

	generalPage = Box::Create(Box::Orientation::VERTICAL, 5);
	newBut = Button::Create("New");
	loadBut = Button::Create("Load");
	saveBut = Button::Create("Save");
	propsBut = Button::Create("Properties");
	allBut = Button::Create("Select All");
	noneBut = Button::Create("Clear Selection");
	setBut = RadioButton::Create("Set");
	setBut->SetActive(true);
	selBut = RadioButton::Create("Select");
	selBut->SetGroup(setBut->GetGroup());
    colsBut = RadioButton::Create("Collisions");
    colsBut->SetGroup(setBut->GetGroup());
    itmBut = RadioButton::Create("Items");
    itmBut->SetGroup(setBut->GetGroup());
    lightBut = RadioButton::Create("Lights");
    lightBut->SetGroup(setBut->GetGroup());
    aiBut = RadioButton::Create("AI");
    aiBut->SetGroup(setBut->GetGroup());
    spwnBut = RadioButton::Create("Spawns");
    spwnBut->SetGroup(setBut->GetGroup());
    addLayerBut = Button::Create("Add Layer");
    delLayerBut = Button::Create("Delete Layer");

    Box::Ptr genCtrlBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    genCtrlBox->Pack(newBut, false, false);
    genCtrlBox->Pack(loadBut, false, false);
    genCtrlBox->Pack(saveBut, false, false);
    genCtrlBox->Pack(propsBut, false, false);
    genCtrlBox->Pack(noneBut, false, false);
    genCtrlBox->Pack(allBut, false, false);
    generalPage->Pack(genCtrlBox, false, false);
    generalPage->Pack(Separator::Create(), false, false);

    Box::Ptr toolTypeBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    toolTypeBox->Pack(Label::Create("Mode:"), false, false);
    toolTypeBox->Pack(setBut, false, false);
    toolTypeBox->Pack(selBut, false, false);
    toolTypeBox->Pack(colsBut, false, false);
    toolTypeBox->Pack(itmBut, false, false);
    toolTypeBox->Pack(lightBut, false, false);
    toolTypeBox->Pack(aiBut, false, false);
    toolTypeBox->Pack(spwnBut, false, false);
    generalPage->Pack(toolTypeBox, false, false);
    generalPage->Pack(Separator::Create(), false, false);

    Box::Ptr layerButBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    layerButBox->Pack(addLayerBut, true, true);
    layerButBox->Pack(delLayerBut, true, true);
    generalPage->Pack(layerButBox, false, false);
    generalPage->Pack(Separator::Create(), false, false);

    layerBox = Box::Create(Box::Orientation::VERTICAL, 5);
    layerBox->Pack(RadioButton::Create("Layer 1"));
    layerBox->Pack(RadioButton::Create("Layer 2"));
    layerBox->Pack(RadioButton::Create("Layer 3"));
    generalPage->Pack(layerBox, false, false);
    tabs->AppendPage(generalPage, Label::Create("General"));

    tilesPage = Box::Create(Box::Orientation::VERTICAL,5);
    tilesPageScroll = ScrolledWindow::Create();
    tilesPageScroll->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    Box::Ptr tilesCtrl = Box::Create(Box::Orientation::HORIZONTAL,5);
    addTileBut = Button::Create("Add");
    addTileBut->GetSignal(Widget::OnLeftClick).Connect( [me] { me->addTile(); });
    tilesCtrl->Pack(addTileBut,false,false);
    delTileBut = Button::Create("Delete");
    tilesCtrl->Pack(delTileBut,false,false);
    tilesPage->Pack(tilesCtrl,false,false);
    tilesPage->Pack(Separator::Create(),false,false);
    noTileBut = RadioButton::Create("None");
    tileBox.addWidget(noTileBut);
    tileBox.setParent(tilesPageScroll);
    tilesPage->Pack(tilesPageScroll);
    tabs->AppendPage(tilesPage, Label::Create("Tiles"));

    parent->AppendPage(container, Label::Create("Map Editor"));
}

void MapEditor::addTile() {
	RadioButton::Ptr but = RadioButton::Create("New tile");
	but->SetGroup(noTileBut->GetGroup());
	tileBox.addWidget(but);
}
