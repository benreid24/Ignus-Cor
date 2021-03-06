#include <iostream>
#include <direct.h>

#include "Editor/MapEditor.hpp"
#include "Editor/Helpers/ScriptEditorWindow.hpp"
#include "Editor/Helpers/FilePicker.hpp"
#include "Shared/Util/Util.hpp"
#include "Shared/Util/File.hpp"
#include "Editor/GUI/ResizableImage.hpp"
#include "Shared/Media/Animation.hpp"
#include "Shared/Properties.hpp"
#include "Editor/GUI/Form.hpp"
#include "Editor/Helpers/MenuGenerators.hpp"
#include "Shared/Items/ItemDB.hpp"
#include "Shared/Entities/EntityManager.hpp"

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
	curTool = Set;
	selCorner1.x = selCorner2.x = -1;

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
	propsBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->editProperties(); });
	allBut = Button::Create("Select All");
	allBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->selectAll(); });
	noneBut = Button::Create("Clear Selection");
	noneBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->clearSelection(); });
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
    spwnrBut = RadioButton::Create("Spawners");
    spwnrBut->SetGroup(setBut->GetGroup());
    spwnrBut->GetSignal(RadioButton::OnToggle).Connect( [me] { me->updateTool(); });
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
    toolTypeBox->Pack(spwnrBut, false, false);
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
		collisionButs[i]->GetSignal(ToggleButton::OnToggle).Connect( [me,i] { me->updateSelected("col",i); });
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
	string f = File::getFile("Image", "png", false, false);
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
	string path = File::getFolder();
	vector<string> files = File::listDirectory(path, "png", true);
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

void MapEditor::addAnim() {
	MapEditor* me = this;
	string f = File::getFile("Animation", "anim", false, false);
	if (f.size()==0)
		return;

	//Determine if we need the spritesheet
	AnimationSource anim(f);
	string baseName = anim.getSpritesheetFilename();
	if (!File::exists(Properties::SpriteSheetPath+baseName)) {
		baseName = File::getFile(baseName, "png", false, false);
		if (baseName.size()==0)
			return;
	}
	else
		baseName = "NONE";

	int id = tileset.addAnim(f,baseName);
	AnimationReference animSrc = tileset.getAnimation(id);
	Sprite spr = animSrc->getFrame(0,Vector2f(0,0), false)[0];
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
	MapEditor* me = this;
	FilePicker picker(desktop, owner, "Animation", Properties::AnimationPath, "anim");

	if (picker.pickFile()) {
		string file = picker.getChoice();

		//Determine if we need the spritesheet
		AnimationSource anim(Properties::AnimationPath+file);
		string baseName = "NONE";
		if (!anim.spritesheetFound()) {
			baseName = File::getFile(baseName, "png", false, false);
			if (baseName.size()==0)
				return;
		}

		int id = tileset.addAnim(Properties::AnimationPath+file,baseName);
		AnimationReference animSrc = tileset.getAnimation(id);
		Sprite spr = animSrc->getFrame(0,Vector2f(0,0), false)[0];
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

void MapEditor::newMap() {
	owner->SetState(Widget::State::INSENSITIVE);

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
				EntityManager::get().clear();
				layerButtons.clear();

				mapData = new Map(name,mapFolder+"/"+name+".map",Vector2i(w,h),tileset,nLayers,firstY,firstTop);
				layerButtons.setLayers(nLayers);
				break;
			}
			goPressed = false;
        }
        if (cancelPressed)
			break;
		desktop.BringToFront(window);

        sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();

		sleep(milliseconds(30));
	}

    desktop.Remove(window);
    owner->SetState(Widget::State::NORMAL);
}

void MapEditor::loadMap() {
	FilePicker picker(desktop, owner, "Map", Properties::MapPath, "map");

	if (picker.pickFile()) {
		const std::string file = picker.getChoice();
		if (mapData!=nullptr) {
			delete mapData;
		}
		mapFolder = File::getPath(file); 
		EntityManager::get().clear();
		layerButtons.clear();
		mapData = new Map(file, tileset);
		layerButtons.setLayers(mapData->getLayerCount());
	}
}

void MapEditor::editProperties() {
	if (!mapData)
		return;

	owner->SetState(Widget::State::INSENSITIVE);
    sfg::Window::Ptr window = sfg::Window::Create();
    window->SetTitle("Map Properties");
    Box::Ptr winBox = Box::Create(Box::Orientation::VERTICAL,5);
	window->Add(winBox);
	desktop.Add(window);

	Form form;
	Button::Ptr loadButton(Button::Create("Pick Onload")), unloadButton(Button::Create("Pick Unload"));
	Button::Ptr plstButton(Button::Create("Pick Playlist")), saveButton(Button::Create("Save")), cancelButton(Button::Create("Cancel"));
	bool cancelPressed(false), savePressed(false), plstPressed(false), loadPressed(false), unloadPressed(false);
	cancelButton->GetSignal(Button::OnLeftClick).Connect( [&cancelPressed] { cancelPressed = true; });
	saveButton->GetSignal(Button::OnLeftClick).Connect( [&savePressed] { savePressed = true; });
	plstButton->GetSignal(Button::OnLeftClick).Connect( [&plstPressed] { plstPressed = true; });
	loadButton->GetSignal(Button::OnLeftClick).Connect( [&loadPressed] { loadPressed = true; });
	unloadButton->GetSignal(Button::OnLeftClick).Connect( [&unloadPressed] { unloadPressed = true; });

	form.addField("w", "Width: ", 80, intToString(mapData->getSize().x));
	form.addField("h", "Height: ", 80, intToString(mapData->getSize().y));
	form.addField("ol", "Onload Script: ", 160, mapData->getLoadScript());
	form.addField("ul", "Unload Script: ", 160, mapData->getUnloadScript());
	form.addField("fy", "First y-sort: ", 80, intToString(mapData->getFirstYSortLayer()));
	form.addField("ft", "First Top: ", 80, intToString(mapData->getFirstTopLayer()));
	form.addField("p", "Playlist: ", 160, mapData->getMusic());
	form.addField("o", "Light Override: ", 80, intToString(mapData->getLightOverride()));
	form.addField("wf", "Weather Freq (0-100): ", 80, intToString(mapData->getWeatherFrequency()));
	form.addToParent(winBox);

	Box::Ptr box = Box::Create(Box::Orientation::HORIZONTAL,5);
    ComboBox::Ptr weatherEntry = ComboBox::Create();
    weatherEntry->AppendItem("None"); //0. Order matters! See Weather::Type
    weatherEntry->AppendItem("Random");
    weatherEntry->AppendItem("Light Rain");
    weatherEntry->AppendItem("Light Rain Thunder");
    weatherEntry->AppendItem("Hard Rain");
    weatherEntry->AppendItem("Hard Rain Thunder");
    weatherEntry->AppendItem("Light Snow");
    weatherEntry->AppendItem("Light Snow Thunder");
    weatherEntry->AppendItem("Hard Snow");
    weatherEntry->AppendItem("Hard Snow Thunder");
    weatherEntry->AppendItem("Light Fog");
    weatherEntry->AppendItem("Thick Fog");
    weatherEntry->AppendItem("Sunny");
    weatherEntry->AppendItem("Sandstorm");
    weatherEntry->AppendItem("Random Rain");
    weatherEntry->AppendItem("Random Snow");
    weatherEntry->AppendItem("Random Desert");
    weatherEntry->SelectItem(mapData->getWeatherType());
    box->Pack(Label::Create("Weather: "),false,false);
    box->Pack(weatherEntry,false,false);
    winBox->Pack(box,false,false);

    box = Box::Create(Box::Orientation::HORIZONTAL,5);
    RadioButton::Ptr topBut = RadioButton::Create("Top");
	topBut->SetActive(true);
	RadioButton::Ptr botBut = RadioButton::Create("Bottom");
	botBut->SetGroup(topBut->GetGroup());
    RadioButton::Ptr leftBut = RadioButton::Create("Left");
    leftBut->SetActive(true);
    RadioButton::Ptr rightBut = RadioButton::Create("Right");
    rightBut->SetGroup(leftBut->GetGroup());
    box->Pack(Label::Create("Add/Remove To/From: "));
    box->Pack(topBut,false,false);
    box->Pack(botBut,false,false);
    box->Pack(leftBut,false,false);
    box->Pack(rightBut,false,false);
    winBox->Pack(box,false,false);

	Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL,5);
	butBox->Pack(plstButton,false,false);
	butBox->Pack(loadButton,false,false);
	butBox->Pack(unloadButton,false,false);
	butBox->Pack(saveButton,false,false);
	butBox->Pack(cancelButton,false,false);
	winBox->Pack(butBox,false,false);

	while (sfWindow.isOpen()) {
		Event wv;
		while (sfWindow.pollEvent(wv)) {
			desktop.HandleEvent(wv);

			if (wv.type==Event::Closed)
				sfWindow.close();
		}
        desktop.Update(30/1000);
        form.update();

        if (plstPressed) {
			plstPressed = false;
			FilePicker picker(desktop, owner, "Playlist", Properties::PlaylistPath, "plst");
			if (picker.pickFile())
                form.setField("p",picker.getChoice());
        }
        if (savePressed) {
			mapData->getMusic() = form.getField("p");
			mapData->setFirstTopLayer(form.getFieldAsInt("ft"));
			mapData->setFirstYSortLayer(form.getFieldAsInt("fy"));
			mapData->setLightingOverride(form.getFieldAsInt("o"));
			mapData->getLoadScript() = form.getField("ol");
			mapData->getUnloadScript() = form.getField("ul");
			mapData->getWeatherFrequency() = form.getFieldAsInt("wf");
			mapData->setWeather(weatherEntry->GetSelectedItem());
			mapData->resize(Vector2i(form.getFieldAsInt("w"),form.getFieldAsInt("h")),topBut->IsActive(),leftBut->IsActive());
			break;
        }
        if (loadPressed || unloadPressed) {
			FilePicker picker(desktop, owner, "Script", Properties::ScriptPath, Properties::ScriptExtension);
			if (picker.pickFile()) {
				if (loadPressed)
					form.setField("ol", picker.getChoice());
				else
					form.setField("ul", picker.getChoice());
			}
			loadPressed = unloadPressed = false;
        }
        if (cancelPressed)
			break;

        desktop.BringToFront(window);
        sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();
		sleep(milliseconds(30));
    }

    desktop.Remove(window);
    owner->SetState(Widget::State::NORMAL);
}

void MapEditor::mapEventHandler(Vector2i pos) {
	MapEvent defaultEvent;
	defaultEvent.type = MapEvent::LoadMap;
	defaultEvent.position = pos;
	defaultEvent.size = Vector2i(1,1);
	defaultEvent.maxRuns = 0;
	defaultEvent.triggerType = MapEvent::StepIn;
	defaultEvent.spawnName = "main";
	defaultEvent.triggerDir = EntityPosition::Any;
	bool newEvt = false;

	MapEvent::Ptr evt = mapData->getEvent(pos.x,pos.y);
	if (!evt) {
		mapData->addEvent(defaultEvent);
		evt = mapData->getEvent(pos.x,pos.y);
		newEvt = true;
	}

	owner->SetState(Widget::State::INSENSITIVE);
    sfg::Window::Ptr window = sfg::Window::Create();
    window->SetTitle("Map Event");
    Box::Ptr winBox = Box::Create(Box::Orientation::VERTICAL,5);
	window->Add(winBox);
	desktop.Add(window);

    Form form;
    Button::Ptr pickButton(Button::Create("Pick/Edit Script")),
			    saveButton(Button::Create("Save")),
				cancelButton(Button::Create("Cancel")),
				delButton(Button::Create("Delete"));
	bool cancelPressed(false), savePressed(false), delPressed(false), pickPressed(false);
	
	pickButton->GetSignal(Button::OnLeftClick).Connect( [&pickPressed] { pickPressed = true; });
	delButton->GetSignal(Button::OnLeftClick).Connect( [&delPressed] { delPressed= true; });
	cancelButton->GetSignal(Button::OnLeftClick).Connect( [&cancelPressed] { cancelPressed = true; });
	saveButton->GetSignal(Button::OnLeftClick).Connect( [&savePressed] { savePressed = true; });

	Box::Ptr box = Box::Create(Box::Orientation::HORIZONTAL,5);
    ComboBox::Ptr typeEntry = ComboBox::Create();
    typeEntry->AppendItem("Run Script");
    typeEntry->AppendItem("Load Map");
    typeEntry->SelectItem(evt->type);
    box->Pack(Label::Create("Type: "),false,false);
    box->Pack(typeEntry,false,false);
    winBox->Pack(box,false,false);

    form.addField("x","X: ",80,intToString(evt->position.x));
    form.addField("y","Y: ",80,intToString(evt->position.y));
    form.addField("w","Width: ",80,intToString(evt->size.x));
    form.addField("h","Height: ",80,intToString(evt->size.y));
    form.addField("r","Max Runs: ",80,intToString(evt->maxRuns));
    form.addField("s","Script: ",250,evt->scriptStr);
	form.addField("sp","Spawn: ",250,evt->spawnName);
    form.addToParent(winBox);

    box = Box::Create(Box::Orientation::HORIZONTAL,5);
    ComboBox::Ptr triggerEntry = ComboBox::Create();
    triggerEntry->AppendItem("On Load");
    triggerEntry->AppendItem("On Enter");
    triggerEntry->AppendItem("On Leave");
    triggerEntry->AppendItem("On Enter or Leave");
    triggerEntry->AppendItem("While In");
	triggerEntry->AppendItem("On Interact");
    triggerEntry->SelectItem(evt->triggerType);
    box->Pack(Label::Create("Trigger: "),false,false);
    box->Pack(triggerEntry,false,false);
    winBox->Pack(box,false,false);

	Box::Ptr intbox = Box::Create(Box::Orientation::HORIZONTAL,5);
    ComboBox::Ptr dirEntry = ComboBox::Create();
    dirEntry->AppendItem("Up");
    dirEntry->AppendItem("Right");
	dirEntry->AppendItem("Down");
	dirEntry->AppendItem("Left");
	dirEntry->AppendItem("Any");
    dirEntry->SelectItem(evt->triggerDir);
    intbox->Pack(Label::Create("Interact Direction: "),false,false);
    intbox->Pack(dirEntry,false,false);
    winBox->Pack(intbox,false,false);

    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL,5);
    butBox->Pack(pickButton,false,false);
    butBox->Pack(saveButton,false,false);
    butBox->Pack(delButton,false,false);
    butBox->Pack(cancelButton,false,false);
    winBox->Pack(butBox,false,false);

    while (sfWindow.isOpen()) {
		Event wv;
		while (sfWindow.pollEvent(wv)) {
			desktop.HandleEvent(wv);

			if (wv.type==Event::Closed)
				sfWindow.close();
		}

		if (typeEntry->GetSelectedItem() == 0) { // Run Script
			pickButton->SetLabel("Pick/Edit Script");
			form.updateFieldLabel("s", "Script: ");
			form.hideInput("sp");
		}
		else if (typeEntry->GetSelectedItem() == 1) { // Load Map
			pickButton->SetLabel("Pick Map");
			form.updateFieldLabel("s", "Map: ");
			form.showInput("sp");
		}

		if (triggerEntry->GetSelectedItem() == 5) { // On Interact
			intbox->Show(true);
		}
		else {
			intbox->Show(false);
		}

		form.update();
		desktop.Update(30/1000);

        if (savePressed) {
			evt->type = (MapEvent::EventType)typeEntry->GetSelectedItem();
			evt->position.x = form.getFieldAsInt("x");
			evt->position.y = form.getFieldAsInt("y");
			evt->size.x = form.getFieldAsInt("w");
			evt->size.y = form.getFieldAsInt("h");
			evt->maxRuns = form.getFieldAsInt("r");
			evt->scriptStr = form.getField("s");
			evt->newMapName = form.getField("s");
			evt->spawnName = form.getField("sp");
			evt->triggerType = (MapEvent::TriggerType)triggerEntry->GetSelectedItem();
			evt->triggerDir = (EntityPosition::Direction)dirEntry->GetSelectedItem();
			break;
        }
        if (delPressed) {
			mapData->removeEvent(pos.x,pos.y);
			break;
        }
        if (cancelPressed) {
			if (newEvt)
				mapData->removeEvent(pos.x,pos.y);
			break;
        }
		if (pickPressed) {
			pickPressed = false;
			if (typeEntry->GetSelectedItem() == 0) { // Run Script
				ScriptEditorWindow editor(desktop, owner, form.getField("s"), true);
				form.setField("s", editor.getScript());
			}
			else if (typeEntry->GetSelectedItem() == 1) { // Load Map
				FilePicker picker(desktop, owner, "Pick Map", Properties::MapPath, "map");
				if (picker.pickFile())
					form.setField("s", picker.getChoice());
			}
		}

        desktop.BringToFront(window);
        sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();
		sleep(milliseconds(30));
    }

    desktop.Remove(window);
    owner->SetState(Widget::State::NORMAL);
}

void MapEditor::lightHandler(Vector2i pos) {
	Light defaultLight;
	defaultLight.position = Vector2f(pos);
	defaultLight.radius = 75;
	defaultLight.threshold = 40;
	bool newLight = false;

	Light* light = mapData->getLight(pos.x,pos.y);
	if (light==nullptr) {
		mapData->addLight(defaultLight);
		light = mapData->getLight(pos.x,pos.y);
		newLight = true;
	}

	owner->SetState(Widget::State::INSENSITIVE);
    sfg::Window::Ptr window = sfg::Window::Create();
    window->SetTitle("Map Light");
    Box::Ptr winBox = Box::Create(Box::Orientation::VERTICAL,5);
	window->Add(winBox);
	desktop.Add(window);

    Form form;
    Button::Ptr saveButton(Button::Create("Save")), cancelButton(Button::Create("Cancel")), delButton(Button::Create("Delete"));
	bool cancelPressed(false), savePressed(false), delPressed(false);
	delButton->GetSignal(Button::OnLeftClick).Connect( [&delPressed] { delPressed= true; });
	cancelButton->GetSignal(Button::OnLeftClick).Connect( [&cancelPressed] { cancelPressed = true; });
	saveButton->GetSignal(Button::OnLeftClick).Connect( [&savePressed] { savePressed = true; });

    form.addField("x","X: ",80,intToString(light->position.x));
    form.addField("y","Y: ",80,intToString(light->position.y));
    form.addField("r","Radius: ",80,intToString(light->radius));
    form.addField("t","Threshold: ",80,intToString(light->threshold));
    form.addToParent(winBox);

    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL,5);
    butBox->Pack(saveButton,false,false);
    butBox->Pack(delButton,false,false);
    butBox->Pack(cancelButton,false,false);
    winBox->Pack(butBox,false,false);

    while (sfWindow.isOpen()) {
		Event wv;
		while (sfWindow.pollEvent(wv)) {
			desktop.HandleEvent(wv);

			if (wv.type==Event::Closed)
				sfWindow.close();
		}
        desktop.Update(30/1000);
        form.update();

        if (savePressed) {
			light->position.x = form.getFieldAsInt("x");
			light->position.y = form.getFieldAsInt("y");
			light->radius = form.getFieldAsInt("r");
			light->threshold = form.getFieldAsInt("t");
			break;
        }
        if (delPressed) {
			mapData->removeLight(pos.x,pos.y);
			break;
        }
        if (cancelPressed) {
			if (newLight)
				mapData->removeLight(pos.x,pos.y);
			break;
        }

        desktop.BringToFront(window);
        sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();
		sleep(milliseconds(30));
    }

    desktop.Remove(window);
    owner->SetState(Widget::State::NORMAL);
}

void MapEditor::spawnHandler(Vector2i pos) {
	EntitySpawn defaultSpawn;
	defaultSpawn.name = "main";
	defaultSpawn.position.dir = EntityPosition::Direction::Up;
	defaultSpawn.position.coords = Vector2f(pos);
	bool newSpawn = false;

	EntitySpawn* spawn = mapData->getEntitySpawn(pos.x,pos.y);
	if (spawn==nullptr) {
		mapData->addEntitySpawn(defaultSpawn);
		spawn = mapData->getEntitySpawn(pos.x,pos.y);
		newSpawn = true;
	}

	owner->SetState(Widget::State::INSENSITIVE);
    sfg::Window::Ptr window = sfg::Window::Create();
    window->SetTitle("Entity Spawn");
    Box::Ptr winBox = Box::Create(Box::Orientation::VERTICAL,5);
	window->Add(winBox);
	desktop.Add(window);

    Form form;
    Button::Ptr saveButton(Button::Create("Save")), cancelButton(Button::Create("Cancel")), delButton(Button::Create("Delete"));
	bool cancelPressed(false), savePressed(false), delPressed(false);
	delButton->GetSignal(Button::OnLeftClick).Connect( [&delPressed] { delPressed= true; });
	cancelButton->GetSignal(Button::OnLeftClick).Connect( [&cancelPressed] { cancelPressed = true; });
	saveButton->GetSignal(Button::OnLeftClick).Connect( [&savePressed] { savePressed = true; });

	form.addField("n", "Name: ",160,spawn->name);
    form.addField("x","X: ",80,intToString(spawn->position.coords.x));
    form.addField("y","Y: ",80,intToString(spawn->position.coords.y));
    form.addToParent(winBox);

    Box::Ptr box = Box::Create(Box::Orientation::HORIZONTAL,5);
    ComboBox::Ptr dirEntry = ComboBox::Create();
    dirEntry->AppendItem("Up");
    dirEntry->AppendItem("Right");
    dirEntry->AppendItem("Down");
    dirEntry->AppendItem("Left");
    dirEntry->SelectItem(spawn->position.dir);
    box->Pack(Label::Create("Direction: "),false,false);
    box->Pack(dirEntry,false,false);
    winBox->Pack(box,false,false);

    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL,5);
    butBox->Pack(saveButton,false,false);
    butBox->Pack(delButton,false,false);
    butBox->Pack(cancelButton,false,false);
    winBox->Pack(butBox,false,false);

    while (sfWindow.isOpen()) {
		Event wv;
		while (sfWindow.pollEvent(wv)) {
			desktop.HandleEvent(wv);

			if (wv.type==Event::Closed)
				sfWindow.close();
		}
        desktop.Update(30/1000);
        form.update();

        if (savePressed) {
			savePressed = false;
			if (form.getField("n")!="prev" && form.getField("n").size()>0) {
				spawn->name = form.getField("n");
				spawn->position.coords.x = form.getFieldAsInt("x");
				spawn->position.coords.y = form.getFieldAsInt("y");
				spawn->position.dir = EntityPosition::Direction(dirEntry->GetSelectedItem());
			}
			break;
        }
        if (delPressed) {
			mapData->removeEntitySpawn(pos.x,pos.y);
			break;
        }
        if (cancelPressed) {
			if (newSpawn)
				mapData->removeEntitySpawn(pos.x,pos.y);
			break;
        }

        desktop.BringToFront(window);
        sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();
		sleep(milliseconds(30));
    }

    desktop.Remove(window);
    owner->SetState(Widget::State::NORMAL);
}

void MapEditor::spawnerHandler(sf::Vector2i pos) {
	MapSpawner defaultSpawn;
	defaultSpawn.position = pos;
	defaultSpawn.coolDown = 10;
	defaultSpawn.frequency = 60;
	defaultSpawn.numActiveLimit = 15;
	defaultSpawn.spawnLimit = 100;
	bool newSpawn = false;

	MapSpawner* spawn = mapData->getMapSpawner(pos.x,pos.y);
	if (spawn==nullptr) {
		mapData->addMapSpawner(defaultSpawn);
		spawn = mapData->getMapSpawner(pos.x,pos.y);
		newSpawn = true;
	}

	owner->SetState(Widget::State::INSENSITIVE);
    sfg::Window::Ptr window = sfg::Window::Create();
    window->SetTitle("Spawner");
    Box::Ptr winBox = Box::Create(Box::Orientation::VERTICAL,5);
	window->Add(winBox);
	desktop.Add(window);

    Form form;
    Button::Ptr saveButton(Button::Create("Save")), cancelButton(Button::Create("Cancel")), delButton(Button::Create("Delete")), pickButton(Button::Create("Pick File"));
	bool cancelPressed(false), savePressed(false), delPressed(false), pickPressed(false);
	delButton->GetSignal(Button::OnLeftClick).Connect( [&delPressed] { delPressed= true; });
	cancelButton->GetSignal(Button::OnLeftClick).Connect( [&cancelPressed] { cancelPressed = true; });
	saveButton->GetSignal(Button::OnLeftClick).Connect( [&savePressed] { savePressed = true; });
	pickButton->GetSignal(Button::OnLeftClick).Connect( [&pickPressed] { pickPressed = true; });

	form.addField("f", "File: ",160,spawn->templateFile);
    form.addField("x","X: ",80,intToString(spawn->position.x));
    form.addField("y","Y: ",80,intToString(spawn->position.y));
    form.addField("al","Max Active: ",80,intToString(spawn->numActiveLimit));
    form.addField("ml","Max to Spawn: ",80,intToString(spawn->spawnLimit));
    form.addField("fr","Frequency (0-100): ",80,intToString(spawn->frequency));
    form.addField("c","Cooldown (s): ",80,intToString(spawn->coolDown));
    form.addToParent(winBox);

    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL,5);
    butBox->Pack(pickButton,false,false);
    butBox->Pack(saveButton,false,false);
    butBox->Pack(delButton,false,false);
    butBox->Pack(cancelButton,false,false);
    winBox->Pack(butBox,false,false);

    while (sfWindow.isOpen()) {
		Event wv;
		while (sfWindow.pollEvent(wv)) {
			desktop.HandleEvent(wv);

			if (wv.type==Event::Closed)
				sfWindow.close();
		}
        desktop.Update(30/1000);
        form.update();

        if (savePressed) {
			spawn->templateFile = form.getField("f");
			spawn->position.x = form.getFieldAsInt("x");
			spawn->position.y = form.getFieldAsInt("y");
			spawn->numActiveLimit = form.getFieldAsInt("al");
			spawn->spawnLimit = form.getFieldAsInt("ml");
			spawn->frequency = form.getFieldAsInt("fr");
			spawn->coolDown = form.getFieldAsInt("c");
			break;
        }
        if (delPressed) {
			mapData->removeMapSpawner(pos.x,pos.y);
			break;
        }
        if (cancelPressed) {
			if (newSpawn)
				mapData->removeMapSpawner(pos.x,pos.y);
			break;
        }
        if (pickPressed) {
			pickPressed = false;
			FilePicker picker(desktop, owner, "Spawner", Properties::SpawnerPath, "spnr");
			if (picker.pickFile())
				form.setField("f", picker.getChoice());
        }

        desktop.BringToFront(window);
        sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();
		sleep(milliseconds(30));
    }

    desktop.Remove(window);
    owner->SetState(Widget::State::NORMAL);
}

void MapEditor::itemHandler(sf::Vector2i pos) {
	bool newItem = false;
	if (ItemDB::get().getItems().size()==0) {
		cout << "No items!" << endl;
		return;
	}

	MapItem* item = mapData->getItem(pos);
	if (item==nullptr)
		newItem = true;

	owner->SetState(Widget::State::INSENSITIVE);
    sfg::Window::Ptr window = sfg::Window::Create();
    window->SetTitle("Map Item");
    Box::Ptr winBox = Box::Create(Box::Orientation::VERTICAL,5);
	window->Add(winBox);
	desktop.Add(window);

    Form form;
    Button::Ptr saveButton(Button::Create("Save")), cancelButton(Button::Create("Cancel")), delButton(Button::Create("Delete"));
	bool cancelPressed(false), savePressed(false), delPressed(false);
	delButton->GetSignal(Button::OnLeftClick).Connect( [&delPressed] { delPressed= true; });
	cancelButton->GetSignal(Button::OnLeftClick).Connect( [&cancelPressed] { cancelPressed = true; });
	saveButton->GetSignal(Button::OnLeftClick).Connect( [&savePressed] { savePressed = true; });

    form.addField("x","X: ",80,newItem?(intToString(pos.x)):(intToString(item->position.x)));
    form.addField("y","Y: ",80,newItem?(intToString(pos.y)):(intToString(item->position.y)));
    form.addToParent(winBox);

    Box::Ptr box = Box::Create(Box::Orientation::HORIZONTAL,5);
    ComboBox::Ptr catEntry = generateItemCategoryFilter();
    box->Pack(Label::Create("Type filter: "),false,false);
    box->Pack(catEntry,false,false);
    winBox->Pack(box,false,false);

    box = Box::Create(Box::Orientation::HORIZONTAL,5);
    map<int,int> comboIdMap, idComboMap;
    ComboBox::Ptr idEntry = generateItemSelector(comboIdMap, idComboMap);
    if (!newItem)
		idEntry->SelectItem(idComboMap[item->itemId]);
	else
		idEntry->SelectItem(0);
    box->Pack(Label::Create("Item: "),false,false);
    box->Pack(idEntry,false,false);
    winBox->Pack(box,false,false);
    integrateItemFilterToList(catEntry, idEntry, &comboIdMap, &idComboMap);

    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL,5);
    butBox->Pack(saveButton,false,false);
    butBox->Pack(delButton,false,false);
    butBox->Pack(cancelButton,false,false);
    winBox->Pack(butBox,false,false);

    while (sfWindow.isOpen()) {
		Event wv;
		while (sfWindow.pollEvent(wv)) {
			desktop.HandleEvent(wv);

			if (wv.type==Event::Closed)
				sfWindow.close();
		}
        desktop.Update(30/1000);
        form.update();

        if (savePressed) {
			if (!newItem) {
				item->position.x = form.getFieldAsInt("x");
				item->position.y = form.getFieldAsInt("y");
				item->itemId = comboIdMap[idEntry->GetSelectedItem()];
				mapData->updateItem(item);
			}
			else
				mapData->addItem(comboIdMap[idEntry->GetSelectedItem()], Vector2i(form.getFieldAsInt("x"),form.getFieldAsInt("y")));
			break;
        }
        if (delPressed) {
			mapData->removeItem(pos);
			break;
        }
        if (cancelPressed) {
			if (newItem)
				mapData->removeItem(pos);
			break;
        }

        desktop.BringToFront(window);
        sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();
		sleep(milliseconds(30));
    }

    desktop.Remove(window);
    owner->SetState(Widget::State::NORMAL);
}
