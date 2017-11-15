#include "Editor/ItemDbEditor.hpp"
#include "Shared/Properties.hpp"
using namespace sfg;
using namespace sf;
using namespace std;

ItemDbEditor::ItemDbEditor(Desktop& dk, Notebook::Ptr parent) : desktop(dk), owner(parent) {
	container = Box::Create(Box::Orientation::VERTICAL,2);
	container->SetRequisition(Vector2f(Properties::ScreenWidth-50,Properties::ScreenHeight-50));
	data = new CellTable(container,"Items");
	data->appendColumn("Name");
	data->appendColumn("Description");
	data->appendColumn("Value");
	data->appendColumn("Effect", {"Fire", "Air", "Water", "Earth"});
	data->appendColumn("Intensity");
	data->appendColumn("MapImg");
	data->appendColumn("MenuImg");
	owner->AppendPage(container,Label::Create("Items"));
}
