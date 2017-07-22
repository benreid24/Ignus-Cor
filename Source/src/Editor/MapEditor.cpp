#include <iostream>
#include "Editor/MapEditor.hpp"
#include "Shared/Util/Util.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/GUI/ResizableImage.hpp"
#include "Shared/Media/Animation.hpp"
#include "Shared/Properties.hpp"
using namespace sfg;
using namespace sf;
using namespace std;

MapEditor::MapEditor(Desktop& dk, Notebook::Ptr parent) : tileBox(Box::Orientation::HORIZONTAL,5,6), animBox(Box::Orientation::HORIZONTAL,5,6), desktop(dk), owner(parent) {
	MapEditor* me = this;
	mapData = nullptr;
	entityManager = nullptr;

	container = Box::Create();
	tabs = Notebook::Create();
	tabs->SetRequisition(Vector2f(300,860));
	mapArea = Canvas::Create();
	mapArea->SetRequisition(Vector2f(1300,860));
	container->Pack(tabs);
	container->Pack(mapArea);
	mapAreaTarget.create(Properties::ScreenWidth,Properties::ScreenHeight);
	mapAreaSprite.setTexture(mapAreaTarget.getTexture());

	generalPage = Box::Create(Box::Orientation::VERTICAL, 5);
	newBut = Button::Create("New");
	newBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->newMap(); });
	loadBut = Button::Create("Load");
	loadBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->loadMap(); });
	saveBut = Button::Create("Save");
	saveBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->save(); });
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
    generalPage->Pack(layerBox, false, false);
    tabs->AppendPage(generalPage, Label::Create("General"));

    tilesPage = Box::Create(Box::Orientation::VERTICAL,5);
    tilesPageScroll = ScrolledWindow::Create();
    tilesPageScroll->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    Box::Ptr tilesCtrl = Box::Create(Box::Orientation::HORIZONTAL,5);
    addTileBut = Button::Create("Add");
    addTileBut->GetSignal(Widget::OnLeftClick).Connect( [me] { me->addTile(); });
    tilesCtrl->Pack(addTileBut,false,false);
    addTileFolderBut = Button::Create("Add Folder");
    addTileFolderBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->addTileFolder(); });
    tilesCtrl->Pack(addTileFolderBut,false,false);
    delTileBut = Button::Create("Delete");
    delTileBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->removeTile(); });
    tilesCtrl->Pack(delTileBut,false,false);
    tilesPage->Pack(tilesCtrl,false,false);
    tilesPage->Pack(Separator::Create(),false,false);
    noTileBut = ToggleButton::Create("None");
    noTileBut->SetActive(true);
    noTileBut->GetSignal(ToggleButton::OnToggle).Connect( [me] { me->updateSelected("tile", 0); });
    tileButs[0] = noTileBut;
    tileBox.addWidget(noTileBut);
    tileBox.setParent(tilesPageScroll);
    tilesPage->Pack(tilesPageScroll);
    tabs->AppendPage(tilesPage, Label::Create("Tiles"));
    selectedTile = 0;

    animPage = Box::Create(Box::Orientation::VERTICAL,5);
    animPageScroll = ScrolledWindow::Create();
    animPageScroll->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    Box::Ptr animCtrl = Box::Create(Box::Orientation::HORIZONTAL,5);
    addAnimBut = Button::Create("Add");
    addAnimBut->GetSignal(Widget::OnLeftClick).Connect( [me] { me->addAnim(); });
    animCtrl->Pack(addAnimBut,false,false);
    delAnimBut = Button::Create("Delete");
    delAnimBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->removeAnim(); });
    animCtrl->Pack(delAnimBut,false,false);
    animPage->Pack(animCtrl,false,false);
    animPage->Pack(Separator::Create(),false,false);
    noAnimBut = ToggleButton::Create("None");
    noAnimBut->SetActive(true);
    noAnimBut->GetSignal(ToggleButton::OnToggle).Connect( [me] { me->updateSelected("anim", 0); });
    animButs[0] = noAnimBut;
    animBox.addWidget(noAnimBut);
    animBox.setParent(animPageScroll);
    animPage->Pack(animPageScroll);
    tabs->AppendPage(animPage, Label::Create("Animations"));
    selectedAnim = 0;

    syncGuiWithTileset();

    parent->AppendPage(container, Label::Create("Map Editor"));
}

void MapEditor::syncGuiWithTileset() {
	MapEditor* me = this;
	tileButs.clear();
	animButs.clear();
	tileBox.clear();
	animBox.clear();

	vector<int> ids = tileset.getTileIds();
	tileButs[0] = noTileBut;
	tileBox.addWidget(noTileBut);
	for (unsigned int i = 0; i<ids.size(); ++i) {
		int id = ids[i];
		if (id==0)
			continue;
		TextureReference txtr = tileset.getTile(id);
		sf::Image img = txtr->copyToImage();

		ToggleButton::Ptr but = ToggleButton::Create(intToString(id));
		but->GetSignal(ToggleButton::OnToggle).Connect( [me, id] { me->updateSelected("tile", id); });
		ResizableImage::Ptr sfgImg = ResizableImage::Create(img);
		sfgImg->Resize(40,40);
		but->SetImage(sfgImg);

		tileButs[id] = but;
		tileBox.addWidget(but);
	}
	ids = tileset.getAnimIds();
	animButs[0] = noAnimBut;
	animBox.addWidget(noAnimBut);
	for (unsigned int i = 0; i<ids.size(); ++i) {
		int id = ids[i];
		if (id==0)
			continue;
		AnimationReference animSrc = tileset.getAnimation(id);
		Sprite spr = animSrc->getFrame(0,Vector2f(0,0))[0];
		RenderTexture render;
		render.create(spr.getGlobalBounds().width,spr.getGlobalBounds().height);
		render.draw(spr);
		render.display();
		sf::Image img = render.getTexture().copyToImage();

		ToggleButton::Ptr but = ToggleButton::Create(intToString(id));
		but->GetSignal(ToggleButton::OnToggle).Connect( [me, id] { me->updateSelected("anim", id); });
		ResizableImage::Ptr sfgImg = ResizableImage::Create(img);
		sfgImg->Resize(40,40);
		but->SetImage(sfgImg);

		animButs[id] = but;
		animBox.addWidget(but);
	}
}

void MapEditor::removeAnim() {
	if (selectedAnim!=0) {
		tileset.removeAnimation(selectedAnim);
		animBox.removeWidget(animButs[selectedAnim]);
		animButs.erase(selectedAnim);
		selectedAnim--;
		animButs[selectedAnim]->SetActive(true);
	}
}

void MapEditor::removeTile() {
	if (selectedTile!=0) {
		tileset.removeTile(selectedTile);
		tileBox.removeWidget(tileButs[selectedTile]);
		tileButs.erase(selectedTile);
		selectedTile--;
		tileButs[selectedTile]->SetActive(true);
	}
}

void MapEditor::save() {
	tileset.save();
	if (mapData!=nullptr)
		mapData->save(Properties::MapPath+mapFolder+"/"+mapData->getName()+".map");
}
