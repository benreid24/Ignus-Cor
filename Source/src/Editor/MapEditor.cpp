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
	tabs->SetRequisition(Vector2f(300,560));
	mapArea = Canvas::Create();
	mapArea->SetRequisition(Vector2f(Properties::ScreenWidth,Properties::ScreenHeight));
	page = Box::Create(Box::Orientation::VERTICAL, 5);
	container->Pack(page);
	container->Pack(Separator::Create(Separator::Orientation::VERTICAL),false,false);
	container->Pack(mapArea);
	mapAreaTarget.create(Properties::ScreenWidth,Properties::ScreenHeight);
	mapAreaSprite.setTexture(mapAreaTarget.getTexture());
	mapAreaSprite.setScale(1,-1);
	mapAreaSprite.setPosition(0,Properties::ScreenHeight);

	Box::Ptr genInfoBox = Box::Create(Box::Orientation::HORIZONTAL,15);
	nameLabel = Label::Create("Name: ");
	genInfoBox->Pack(nameLabel,false,false);
	widthLabel = Label::Create("Width: ");
	genInfoBox->Pack(widthLabel,false,false);
	heightLabel = Label::Create("Height: ");
	genInfoBox->Pack(heightLabel,false,false);
	firstyLabel = Label::Create("First Y-Sort Layer: ");
	genInfoBox->Pack(firstyLabel,false,false);
	page->Pack(genInfoBox,false,false);
	genInfoBox = Box::Create(Box::Orientation::HORIZONTAL,15);
	firsttopLabel = Label::Create("First Top Layer: ");
	genInfoBox->Pack(firsttopLabel,false,false);
	musicLabel = Label::Create("Playlist: ");
	genInfoBox->Pack(musicLabel,false,false);
	posLabel = Label::Create("Position: (-,-)");
	genInfoBox->Pack(posLabel,false,false);
	page->Pack(genInfoBox,false,false);
	page->Pack(Separator::Create(),false,false);

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
    addLayerBeforeBut = Button::Create("Add Layer Before");
    addLayerBeforeBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->addLayer(0); });
	addLayerAfterBut = Button::Create("Add Layer After");
    addLayerAfterBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->addLayer(1); });
    delLayerBut = Button::Create("Delete Layer");
    delLayerBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->removeLayer(); });

    Box::Ptr genCtrlBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    genCtrlBox->Pack(newBut, false, false);
    genCtrlBox->Pack(loadBut, false, false);
    genCtrlBox->Pack(saveBut, false, false);
    genCtrlBox->Pack(propsBut, false, false);
    genCtrlBox->Pack(noneBut, false, false);
    genCtrlBox->Pack(allBut, false, false);
    page->Pack(genCtrlBox, false, false);
    page->Pack(Separator::Create(), false, false);

    Box::Ptr toolTypeBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    toolTypeBox->Pack(Label::Create("Mode:"), false, false);
    toolTypeBox->Pack(setBut, false, false);
    toolTypeBox->Pack(selBut, false, false);
    toolTypeBox->Pack(colsBut, false, false);
    toolTypeBox->Pack(itmBut, false, false);
    toolTypeBox->Pack(lightBut, false, false);
    toolTypeBox->Pack(aiBut, false, false);
    toolTypeBox->Pack(spwnBut, false, false);
    page->Pack(toolTypeBox, false, false);
    page->Pack(Separator::Create(), false, false);

    Box::Ptr layerButBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    layerButBox->Pack(addLayerBeforeBut, true, true);
    layerButBox->Pack(addLayerAfterBut, true, true);
    layerButBox->Pack(delLayerBut, true, true);
    page->Pack(layerButBox, false, false);
    page->Pack(Separator::Create(), false, false);
	layerButtons.addToParent(page);

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
    newAnimBut = Button::Create("Create New");
    newAnimBut->GetSignal(Widget::OnLeftClick).Connect( [me] { me->createAnim(); });
    animCtrl->Pack(newAnimBut,false,false);
    addAnimBut = Button::Create("Import From External Source");
    addAnimBut->GetSignal(Widget::OnLeftClick).Connect( [me] { me->addAnim(); });
    animCtrl->Pack(addAnimBut,false,false);
    loadAnimBut = Button::Create("Import From Existing");
    loadAnimBut->GetSignal(Widget::OnLeftClick).Connect( [me] { me->loadAnim(); });
    animCtrl->Pack(loadAnimBut,false,false);
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

    page->Pack(tabs);
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
	if (mapData!=nullptr) {
		string file = Properties::MapPath;
		if (mapFolder.size()>0)
			file += mapFolder+"/";
		file += mapData->getName()+".map";
		mapData->save(file);
	}
}

void MapEditor::updateInfo() {
	if (mapData) {
        nameLabel->SetText("Name: "+mapData->getName());
        widthLabel->SetText("Width: "+intToString(mapData->getSize().x));
        heightLabel->SetText("Height: "+intToString(mapData->getSize().y));
        firstyLabel->SetText("First Y-Sort Layer: "+intToString(mapData->getFirstYSortLayer()));
        firsttopLabel->SetText("First Top Layer: "+intToString(mapData->getFirstTopLayer()));
        musicLabel->SetText("Playlist: "+mapData->getMusic());
        Vector2i pos = Mouse::getPosition(sfWindow);
		if (pos.x>=mapArea->GetAbsolutePosition().x && pos.y>=mapArea->GetAbsolutePosition().y && pos.x<mapArea->GetAbsolutePosition().x+Properties::ScreenWidth && pos.y<mapArea->GetAbsolutePosition().y+Properties::ScreenHeight) {
            pos = getMouseTilePos();
            posLabel->SetText("Position: ("+intToString(pos.x/32)+","+intToString(pos.y/32)+")");
		}
		else
			posLabel->SetText("Position: (-,-)");
	}
}

void MapEditor::update() {
	if (mapData!=nullptr) {
		int moveAmount = Keyboard::isKeyPressed(Keyboard::Space)?(2):(1);
		Vector2f camPos = mapData->getCamera();
		if (Keyboard::isKeyPressed(Keyboard::Right))
			camPos.x += 32*moveAmount;
		if (Keyboard::isKeyPressed(Keyboard::Left))
			camPos.x -= 32*moveAmount;
		if (Keyboard::isKeyPressed(Keyboard::Up))
			camPos.y -= 32*moveAmount;
		if (Keyboard::isKeyPressed(Keyboard::Down))
			camPos.y += 32*moveAmount;
		mapData->setRenderPosition(camPos+Vector2f(Properties::ScreenWidth/2,Properties::ScreenHeight/2));
		mapData->update();

		if (Mouse::isButtonPressed(Mouse::Left)) {
			Vector2i pos = Mouse::getPosition(sfWindow);
			if (pos.x>=mapArea->GetAbsolutePosition().x && pos.y>=mapArea->GetAbsolutePosition().y && pos.x<mapArea->GetAbsolutePosition().x+Properties::ScreenWidth && pos.y<mapArea->GetAbsolutePosition().y+Properties::ScreenHeight)
				mapClicked();
		}
		updateInfo();
	}
}

void MapEditor::render() {
	if (mapData) {
		mapArea->Bind();
		mapAreaTarget.clear();
		mapData->draw(mapAreaTarget,layerButtons.getVisibleLayers());
		mapArea->Draw(mapAreaSprite);
		mapArea->Unbind();
	}
}

Vector2i MapEditor::getMouseTilePos() {
	if (!mapData)
		return Vector2i(-1,-1);

	Vector2i pos = Mouse::getPosition(sfWindow);
	pos -= Vector2i(mapArea->GetAbsolutePosition());
	pos += Vector2i(mapData->getCamera());
	return pos;
}

void MapEditor::mapClicked() {
	if (!mapData)
		return;

	Vector2i pos = getMouseTilePos();
	pos.x /= 32;
	pos.y /= 32;
	bool isAnim = tabs->GetCurrentPage()==1;
	int id = (isAnim)?(selectedAnim):(selectedTile);
	int layer = layerButtons.getCurrentLayer();
	if (layer==-1)
		return;

	mapData->editTile(pos.x,pos.y,layer,id,isAnim);
	cout << "Clicked pos: (" << pos.x << "," << pos.y << ") with selected id: " << id << "\n";
}

void MapEditor::addLayer(int o) {
	if (mapData) {
		int layer = layerButtons.getCurrentLayer();
		if (layer!=-1)
			layer += o;
		layerButtons.addLayer();
		mapData->addLayer(layer);
	}
}

void MapEditor::removeLayer() {
	if (mapData && layerButtons.getCurrentLayer()!=-1) {
		mapData->removeLayer(layerButtons.getCurrentLayer());
		layerButtons.removeLayer();
	}
}
