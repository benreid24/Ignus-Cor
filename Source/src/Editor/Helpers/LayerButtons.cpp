#include "Editor/Helpers/LayerButtons.hpp"
#include "Shared/Util/Util.hpp"
using namespace std;
using namespace sfg;

LayerButtons::LayerButtons() {
	container = Box::Create(Box::Orientation::VERTICAL,5);
}

void LayerButtons::setLayers(int nLayers) {
	clear();
	for (int i = 0; i<nLayers; ++i)
		addLayer();
}

void LayerButtons::clear() {
	container->RemoveAll();
	layerButs.clear();
}

void LayerButtons::addLayer() {
    Layer t;
    t.wrapper = Box::Create(Box::Orientation::HORIZONTAL,5);
    t.label = Label::Create("Layer "+intToString(layerButs.size())+":");
    t.activeBut = RadioButton::Create("Active");
    t.visibleBut = ToggleButton::Create("Visible");
    t.wrapper->Pack(t.label);
    t.wrapper->Pack(t.activeBut);
    t.wrapper->Pack(t.visibleBut);
    if (layerButs.size()>0)
		t.activeBut->SetGroup(layerButs[0].activeBut->GetGroup());
	layerButs.push_back(t);
	container->Pack(t.wrapper,false,false);
}

void LayerButtons::removeLayer() {
	if (layerButs.size()>0) {
		container->Remove(layerButs[layerButs.size()-1].wrapper);
		layerButs.erase(layerButs.begin()+layerButs.size()-1);
	}
}

void LayerButtons::addToParent(Box::Ptr parent) {
	parent->Pack(container,false,false);
}

int LayerButtons::getCurrentLayer() {
	for (unsigned int i = 0; i<layerButs.size(); ++i) {
		if (layerButs[i].activeBut->IsActive())
			return i;
	}
	return -1;
}

vector<int> LayerButtons::getVisibleLayers() {
	vector<int> rv;
	for (unsigned int i = 0; i<layerButs.size(); ++i) {
		if (layerButs[i].visibleBut->IsActive())
			rv.push_back(i);
	}
	return rv;
}
