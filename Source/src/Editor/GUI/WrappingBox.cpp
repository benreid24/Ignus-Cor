#include <Editor/GUI/WrappingBox.hpp>
#include <algorithm>
using namespace sfg;
using namespace sf;
using namespace std;

WrappingBox::WrappingBox(Box::Orientation cDir, double s, int width) {
	cutDir = cDir;
	space = s;
	maxCutSize = width;
	curCutSize = 0;

	rowDir = (cutDir==Box::Orientation::HORIZONTAL)?(Box::Orientation::VERTICAL):(Box::Orientation::HORIZONTAL);
	container = Box::Create(rowDir, space);
	cuts.push_back(Box::Create(cutDir, space));

	container->Pack(cuts[0],false,false);
}

void WrappingBox::clearGui() {
	container->RemoveAll();
	cuts.clear();
	curCutSize = 0;
	cuts.push_back(Box::Create(cutDir, space));
	container->Pack(cuts[0],false,false);
}

void WrappingBox::addWidgetToGui(Widget::Ptr widget) {
	cuts[cuts.size()-1]->Pack(widget, false, false);
	curCutSize++;
	if (curCutSize>=maxCutSize) {
		curCutSize = 0;
		cuts.push_back(Box::Create(cutDir, space));
		container->Pack(cuts[cuts.size()-1],false,false);
	}
}

void WrappingBox::refresh() {
	clearGui();
	for (unsigned int i = 0; i<members.size(); ++i) {
        addWidgetToGui(members[i]);
	}
}

void WrappingBox::setParent(Box::Ptr parent) {
	parent->Pack(container);
}

void WrappingBox::setParent(ScrolledWindow::Ptr parent) {
	parent->AddWithViewport(container);
}

void WrappingBox::addWidget(Widget::Ptr widget) {
	members.push_back(widget);
	addWidgetToGui(widget);
}

void WrappingBox::removeWidget(Widget::Ptr widget) {
    auto i = find(members.begin(), members.end(), widget);
    if (i!=members.end()) {
		members.erase(i);
		refresh();
    }
}

void WrappingBox::clear() {
    clearGui();
    members.clear();
}
