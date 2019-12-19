#include <iostream>
#include "Editor/MapEditor.hpp"
#include "Shared/Util/Util.hpp"
#include "Shared/Util/File.hpp"
#include "Editor/GUI/ResizableImage.hpp"
#include "Shared/Media/Animation.hpp"
#include "Shared/Properties.hpp"
using namespace sfg;
using namespace sf;
using namespace std;

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
            posLabel->SetText("Position: ("+intToString(pos.x)+","+intToString(pos.y)+")");
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
		EntityManager::get().update();

		if (Mouse::isButtonPressed(Mouse::Left)) {
			Vector2i pos = Mouse::getPosition(sfWindow);
			if (pos.x>=mapArea->GetAbsolutePosition().x && pos.y>=mapArea->GetAbsolutePosition().y && pos.x<mapArea->GetAbsolutePosition().x+Properties::ScreenWidth && pos.y<mapArea->GetAbsolutePosition().y+Properties::ScreenHeight)
				mapClicked();
		}
		updateInfo();
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
	else if (spwnrBut->IsActive())
		curTool = Spawners;
}

void MapEditor::render() {
	if (mapData) {
		mapArea->Bind();
		mapAreaTarget.clear();
		vector<int> layers = layerButtons.getVisibleLayers();
		if (tabs->GetCurrentPage()==2)
			layers.push_back(-1);
		IntRect selection(selCorner1.x,selCorner1.y,selCorner2.x-selCorner1.x,selCorner2.y-selCorner1.y);
		if (selCorner1.x<0 || selCorner2.x<0)
			selection.width = selection.height = 0;
		mapData->draw(mapAreaTarget,layers, selection, evtBut->IsActive());
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
	pos.x /= 32;
	pos.y /= 32;

	return pos;
}

void MapEditor::mapClicked() {
	if (!mapData)
		return;
	Vector2i pos = getMouseTilePos();
	Vector2i pixelPos = Mouse::getPosition(sfWindow);
	pixelPos -= Vector2i(mapArea->GetAbsolutePosition());
	pixelPos += Vector2i(mapData->getCamera());

	if (curTool==Set) {
		enum CurTab{
			Tiles = 0,
			Anims = 1,
			Collisions = 2
		}curTab = (CurTab)tabs->GetCurrentPage();
		if (curTab==Collisions) {
			if (selCorner1.x<0 || selCorner2.x<0)
				mapData->setCollision(pos.x,pos.y,selectedCollision);
			else {
				for (int x = selCorner1.x; x<selCorner2.x; ++x) {
					for (int y = selCorner1.y; y<selCorner2.y; ++y) {
						mapData->setCollision(x,y,selectedCollision);
					}
				}
			}
		}
		else {
			bool isAnim = curTab==Anims;
			int id = (isAnim)?(selectedAnim):(selectedTile);
			int layer = layerButtons.getCurrentLayer();
			if (layer==-1)
				return;
			if (selCorner1.x<0 || selCorner2.x<0)
				mapData->editTile(pos.x,pos.y,layer,id,isAnim);
			else {
				for (int x = selCorner1.x; x<selCorner2.x; ++x) {
					for (int y = selCorner1.y; y<selCorner2.y; ++y) {
						mapData->editTile(x,y,layer,id,isAnim);
					}
				}
			}
		}
	}
	else if (curTool==Select) {
		if (selCorner1.x<0)
			selCorner1 = pos;
		else if (selCorner2.x<0) {
			selCorner2.x = max(selCorner1.x,pos.x)+1; //we add 1 to be inclusive
			selCorner2.y = max(selCorner1.y,pos.y)+1;
			selCorner1.x = min(selCorner1.x,pos.x);
            selCorner1.y = min(selCorner1.y,pos.y);
		}
		else {
			selCorner1.x = -1;
			selCorner2.x = -1;
		}
		sleep(milliseconds(120));
	}
	else if (curTool==Events)
		mapEventHandler(pos);
	else if (curTool==Lights)
		lightHandler(pixelPos);
	else if (curTool==Spawns)
		spawnHandler(pos);
	else if (curTool==Spawners)
		spawnerHandler(pos);
	else if (curTool==Items)
		itemHandler(pixelPos);
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

void MapEditor::clearSelection() {
    selCorner1.x = selCorner2.x = -1;
}

void MapEditor::selectAll() {
	if (mapData!=nullptr) {
		selCorner1.x = selCorner1.y = 0;
		selCorner2.x = mapData->getSize().x;
		selCorner2.y = mapData->getSize().y;
	}
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
	else if (type=="anim") {
		if (id!=selectedAnim && animButs[id]->IsActive()) {
			int t = selectedAnim;
			selectedAnim = id;
			animButs[t]->SetActive(false);
		}
		else if (id==selectedAnim && !animButs[id]->IsActive())
			animButs[id]->SetActive(true);
	}
	else {
		if (id!=selectedCollision && collisionButs[id]->IsActive()) {
			int t = selectedCollision;
			selectedCollision = id;
			collisionButs[t]->SetActive(false);
		}
		else if (id==selectedCollision && !collisionButs[id]->IsActive())
			collisionButs[id]->SetActive(true);
	}
}
