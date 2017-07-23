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

void MapEditor::newMap() {
	owner->Show(false);

	sfg::Window::Ptr window = sfg::Window::Create();
	window->SetRequisition(Vector2f(220,240));
	sfg::Box::Ptr winHolder = sfg::Box::Create(Box::Orientation::VERTICAL,5);
	window->SetTitle("New Map");

	Box::Ptr line = Box::Create(Box::Orientation::HORIZONTAL,5);
	Label::Ptr label = Label::Create("Name:");
	Entry::Ptr mapName = Entry::Create();
	mapName->SetRequisition(Vector2f(120,20));
	line->Pack(label,false,false);
	line->Pack(mapName,false,false);
	winHolder->Pack(line,false,false);

	line = Box::Create(Box::Orientation::HORIZONTAL,5);
	label = Label::Create("Subfolder:");
    Entry::Ptr subfolderEntry = Entry::Create();
    subfolderEntry->SetRequisition(Vector2f(120,20));
    line->Pack(label,false,false);
    line->Pack(subfolderEntry,false,false);
    winHolder->Pack(line,false,false);

    line = Box::Create(Box::Orientation::HORIZONTAL,5);
	label = Label::Create("Width:");
    Entry::Ptr width = Entry::Create();
    width->SetRequisition(Vector2f(40,20));
    line->Pack(label,false,false);
    line->Pack(width,false,false);
    winHolder->Pack(line,false,false);

    line = Box::Create(Box::Orientation::HORIZONTAL,5);
	label = Label::Create("Height:");
    Entry::Ptr height = Entry::Create();
    height->SetRequisition(Vector2f(40,20));
    line->Pack(label,false,false);
    line->Pack(height,false,false);
    winHolder->Pack(line,false,false);

    line = Box::Create(Box::Orientation::HORIZONTAL,5);
	label = Label::Create("Number of Layers:");
    Entry::Ptr numLayers = Entry::Create("5");
    numLayers->SetRequisition(Vector2f(40,20));
    line->Pack(label,false,false);
    line->Pack(numLayers,false,false);
    winHolder->Pack(line,false,false);

    line = Box::Create(Box::Orientation::HORIZONTAL,5);
	label = Label::Create("First y-sort Layer:");
    Entry::Ptr ySort = Entry::Create("2");
    ySort->SetRequisition(Vector2f(40,20));
    line->Pack(label,false,false);
    line->Pack(ySort,false,false);
    winHolder->Pack(line,false,false);

    line = Box::Create(Box::Orientation::HORIZONTAL,5);
	label = Label::Create("First Top Layer:");
    Entry::Ptr topLayer = Entry::Create("4");
    topLayer->SetRequisition(Vector2f(40,20));
    line->Pack(label,false,false);
    line->Pack(topLayer,false,false);
    winHolder->Pack(line,false,false);

    bool goPressed = false, cancelPressed = false;
    line = Box::Create(Box::Orientation::HORIZONTAL,5);
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
        if (goPressed) {
			string name = mapName->GetText();
			string subfolder = subfolderEntry->GetText();
			int w = stringToInt(width->GetText());
			int h = stringToInt(height->GetText());
			int nLayers = stringToInt(numLayers->GetText());
			int firstY = stringToInt(ySort->GetText());
			int firstTop = stringToInt(topLayer->GetText());

			if (w!=0 && h!=0 && nLayers!=0 && firstY!=0 && firstTop!=0 && name.size()!=0) {
				save();
				if (mapData!=nullptr)
					delete mapData;
				if (entityManager!=nullptr)
					delete entityManager;
				layerBox->RemoveAll();

                entityManager = new EntityManager();
				mapData = new Map(name,Vector2i(w,h),tileset,&soundEngine,entityManager,nLayers,firstY,firstTop);
				RadioButton::Ptr but = RadioButton::Create("Layer 0");
				layerBox->Pack(but,false,false);
				for (int i = 1; i<nLayers; ++i) {
                    RadioButton::Ptr t = RadioButton::Create("Layer "+intToString(i));
                    t->SetGroup(but->GetGroup());
                    layerBox->Pack(t,false,false);
				}
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
	//
}
