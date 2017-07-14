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
	//
}

void MapEditor::loadMap() {
	//
}
