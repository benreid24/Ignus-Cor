#include <iostream>
#include <direct.h>
#include "Editor/MapEditor.hpp"
#include "Editor/Helpers/FilePicker.hpp"
#include "Shared/Util/Util.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/GUI/ResizableImage.hpp"
#include "Shared/Media/Animation.hpp"
#include "Shared/Properties.hpp"
#include "Shared/GUI/Form.hpp"
using namespace sfg;
using namespace sf;
using namespace std;

///0 = none, 1 = all, 2 = top, 3 = right, 4 = bottom, 5 = left, 6 = topRight, 7 = bottomRight, 8 = bottomLeft, 9 = topLeft
///10 = topBottom, 11 = rightLeft, 12 = noTop, 13 = noRight, 14 = noBottom, 15 = noLeft
namespace {
	const string collisionFiles[16] = {Properties::EditorResources+"none.png",
									   Properties::EditorResources+"all.png",
									   Properties::EditorResources+"top.png",
  									   Properties::EditorResources+"right.png",
	  								   Properties::EditorResources+"bottom.png",
		  							   Properties::EditorResources+"left.png",
									   Properties::EditorResources+"topRight.png",
									   Properties::EditorResources+"bottomRight.png",
									   Properties::EditorResources+"bottomLeft.png",
									   Properties::EditorResources+"topLeft.png",
									   Properties::EditorResources+"topBottom.png",
									   Properties::EditorResources+"leftRight.png",
									   Properties::EditorResources+"noTop.png",
									   Properties::EditorResources+"noRight.png",
									   Properties::EditorResources+"noBottom.png",
									   Properties::EditorResources+"noLeft.png"};
}

MapEditor::MapEditor(Desktop& dk, Notebook::Ptr parent) : tileBox(Box::Orientation::HORIZONTAL,5,6), animBox(Box::Orientation::HORIZONTAL,5,6), collisionBox(Box::Orientation::HORIZONTAL,5,6), desktop(dk), owner(parent) {
	MapEditor* me = this;
	mapData = nullptr;
	entityManager = nullptr;
	curTool = Set;

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
	setBut->GetSignal(RadioButton::OnToggle).Connect( [me] { me->updateTool(); });
	selBut = RadioButton::Create("Select");
	selBut->SetGroup(setBut->GetGroup());
	selBut->GetSignal(RadioButton::OnToggle).Connect( [me] { me->updateTool(); });
    evtBut = RadioButton::Create("Events");
    evtBut->SetGroup(setBut->GetGroup());
    evtBut->GetSignal(RadioButton::OnToggle).Connect( [me] { me->updateTool(); });
    itmBut = RadioButton::Create("Items");
    itmBut->SetGroup(setBut->GetGroup());
    itmBut->GetSignal(RadioButton::OnToggle).Connect( [me] { me->updateTool(); });
    lightBut = RadioButton::Create("Lights");
    lightBut->SetGroup(setBut->GetGroup());
    lightBut->GetSignal(RadioButton::OnToggle).Connect( [me] { me->updateTool(); });
    aiBut = RadioButton::Create("AI");
    aiBut->SetGroup(setBut->GetGroup());
    aiBut->GetSignal(RadioButton::OnToggle).Connect( [me] { me->updateTool(); });
    spwnBut = RadioButton::Create("Spawns");
    spwnBut->SetGroup(setBut->GetGroup());
    spwnBut->GetSignal(RadioButton::OnToggle).Connect( [me] { me->updateTool(); });
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
    toolTypeBox->Pack(evtBut, false, false);
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

    collisionPage = Box::Create();
    collisionButs.resize(16);
    for (int i = 0; i<16; ++i) {
		collisionButs[i] = ToggleButton::Create();
		sf::Image img;
		img.loadFromFile(collisionFiles[i]);
		ResizableImage::Ptr rImg = ResizableImage::Create(img);
		collisionButs[i]->SetImage(rImg);
		collisionBox.addWidget(collisionButs[i]);
    }
    collisionBox.setParent(collisionPage);
    tabs->AppendPage(collisionPage, Label::Create("Collisions"));
    selectedCollision = 0;

    page->Pack(tabs);
    syncGuiWithTileset();

    parent->AppendPage(container, Label::Create("Map Editor"));
}

void MapEditor::addTile() {
	MapEditor* me = this;
	string f = getFilename("Image (*.png)\0 *.png\0\0",false,false);
	if (f.size()==0)
		return;

	int id = tileset.addTile(f);
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

void MapEditor::addTileFolder() {
	MapEditor* me = this;
	string path = getFoldername();
	vector<string> files = listDirectory(path, "png", true);
	for (unsigned int i = 0; i<files.size(); ++i) {
		int id = tileset.addTile(files[i]);
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
}

void MapEditor::createAnim() {
	//TODO - create animation editor
}

void MapEditor::addAnim() {
	MapEditor* me = this;
	string f = getFilename("Animation (*.anim)\0 *.anim\0\0",false,false);
	if (f.size()==0)
		return;

	//Determine if we need the spritesheet
	AnimationSource anim(f);
	string baseName = anim.getSpritesheetFilename();
	if (!FileExists(Properties::SpriteSheetPath+baseName)) {
		baseName = getFilename(string(baseName+" (*.png)\0 *.png\0\0").c_str(),false,false);
		if (baseName.size()==0)
			return;
	}
	else
		baseName = "NONE";

	int id = tileset.addAnim(f,baseName);
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

void MapEditor::loadAnim() {
	owner->Show(false);
	FilePicker picker(desktop, "Animation", Properties::AnimationPath, "anim");

	if (picker.pickFile()) {
		string file = picker.getChoice();
		cout << "Chose " << file << endl;
		//TODO - import anim
	}
	owner->Show(true);
}

void MapEditor::updateSelected(const string& type, int id) {
	if (type=="tile") {
		if (id!=selectedTile && tileButs[id]->IsActive()) {
			int t = selectedTile;
			selectedTile = id;
			tileButs[t]->SetActive(false);
		}
		else if (id==selectedTile && !tileButs[id]->IsActive())
			tileButs[id]->SetActive(true);
	}
	else {
		if (id!=selectedAnim && animButs[id]->IsActive()) {
			int t = selectedAnim;
			selectedAnim = id;
			animButs[t]->SetActive(false);
		}
		else if (id==selectedAnim && !animButs[id]->IsActive())
			animButs[id]->SetActive(true);
	}
}

void MapEditor::updateTool() {
	if (setBut->IsActive())
		curTool = Set;
	else if (selBut->IsActive())
		curTool = Select;
	else if (evtBut->IsActive())
		curTool = Events;
	else if (itmBut->IsActive())
		curTool = Items;
	else if (lightBut->IsActive())
		curTool = Lights;
	else if (spwnBut->IsActive())
		curTool = Spawns;
	else if (aiBut->IsActive())
		curTool = AI;
}

void MapEditor::newMap() {
	owner->Show(false);

	sfg::Window::Ptr window = sfg::Window::Create();
	window->SetRequisition(Vector2f(220,240));
	sfg::Box::Ptr winHolder = sfg::Box::Create(Box::Orientation::VERTICAL,5);
	window->SetTitle("New Map");

    Form form;
    form.addField("name","Name:");
    form.addField("folder","Subfolder:");
    form.addField("width","Width:",40);
    form.addField("height", "Height:",40);
    form.addField("nlayers","Number of Layers",40,"5");
    form.addField("firsty","First y-sort Layer:",40,"2");
    form.addField("firsttop","First Top Layer:",40,"4");
    form.addToParent(winHolder);

    bool goPressed = false, cancelPressed = false;
    Box::Ptr line = Box::Create(Box::Orientation::HORIZONTAL,5);
    Button::Ptr goBut = Button::Create("Create");
    goBut->GetSignal(Button::OnLeftClick).Connect( [&goPressed] { goPressed = true; });
    line->Pack(goBut,false,false);
    Button::Ptr cancelBut = Button::Create("Cancel");
    cancelBut->GetSignal(Button::OnLeftClick).Connect( [&cancelPressed] { cancelPressed = true; });
    line->Pack(cancelBut,false,false);
    winHolder->Pack(line,false,false);

	window->Add(winHolder);
	desktop.Add(window);

	while (sfWindow.isOpen()) {
		Event evt;
		while (sfWindow.pollEvent(evt)) {
			desktop.HandleEvent(evt);

			if (evt.type==Event::Closed)
				sfWindow.close();
		}
        desktop.Update(30/1000);
        form.update();
        if (goPressed || Keyboard::isKeyPressed(Keyboard::Return)) {
			string name = form.getField("name");
			mapFolder = form.getField("folder");
			int w = form.getFieldAsInt("width");
			int h = form.getFieldAsInt("height");
			int nLayers = form.getFieldAsInt("nlayers");
			int firstY = form.getFieldAsInt("firsty");
			int firstTop = form.getFieldAsInt("firsttop");

			if (w!=0 && h!=0 && nLayers!=0 && firstY!=0 && firstTop!=0 && name.size()!=0) {
				save();
				mkdir(string(Properties::MapPath+mapFolder).c_str());
				if (mapData!=nullptr)
					delete mapData;
				if (entityManager!=nullptr)
					delete entityManager;
				layerButtons.clear();

                entityManager = new EntityManager();
				mapData = new Map(name,Vector2i(w,h),tileset,&soundEngine,entityManager,nLayers,firstY,firstTop);
				layerButtons.setLayers(nLayers);
				break;
			}
			goPressed = false;
        }
        if (cancelPressed)
			break;

        sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();

		sleep(milliseconds(30));
	}

    desktop.Remove(window);
    owner->Show(true);
}

void MapEditor::loadMap() {
	owner->Show(false);
	FilePicker picker(desktop, "Map", Properties::MapPath, "map");

	if (picker.pickFile()) {
		if (mapData!=nullptr) {
			save();
			delete mapData;
		}
		if (entityManager!=nullptr)
			delete entityManager;
		layerButtons.clear();
		entityManager = new EntityManager();
		mapData = new Map(picker.getChoice(),tileset,entityManager,&soundEngine);
		layerButtons.setLayers(mapData->getLayerCount());
	}
	owner->Show(true);
}
