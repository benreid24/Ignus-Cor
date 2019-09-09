#include <SFGUI/SFGUI.hpp>
#include "Editor/ItemDbEditor.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Data/ItemDB.hpp"
#include "Shared/GUI/Form.hpp"
#include "Editor/Helpers/FilePicker.hpp"
#include "Editor/Helpers/MenuGenerators.hpp"
#include "Shared/Util/Util.hpp"
using namespace sfg;
using namespace sf;
using namespace std;

extern RenderWindow sfWindow;
extern SFGUI sfgui;

ItemDbEditor::ItemDbEditor(Desktop& dk, Notebook::Ptr parent) : desktop(dk), owner(parent) {
	ItemDbEditor* me = this;

	container = Box::Create(Box::Orientation::VERTICAL,2);
	container->SetRequisition(Vector2f(Properties::ScreenWidth-50,Properties::ScreenHeight-50));

	Button::Ptr saveBut = Button::Create("Save"), appBut = Button::Create("New");
    saveBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->save(); });
    appBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->doItem(); });
    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    butBox->Pack(saveBut,false,false);
    butBox->Pack(appBut,false,false);
    container->Pack(butBox,false,false);

    Box::Ptr cellBox = Box::Create();
	data = new CellTable(cellBox);
	cellBox->Pack(Separator::Create(Separator::Orientation::VERTICAL),false,false);
    container->Pack(cellBox,false,false);
	owner->AppendPage(container,Label::Create("Items"));
	updateGui();
}

void ItemDbEditor::save() {
	ItemDB::get().save();
}

void ItemDbEditor::doItem(int id) {
	Item::ConstPtr item = ItemDB::get().getItem(id);

	owner->SetState(Widget::State::INSENSITIVE);
    sfg::Window::Ptr window = sfg::Window::Create();
    window->SetTitle("Item");
    Box::Ptr winBox = Box::Create(Box::Orientation::VERTICAL,5);
	window->Add(winBox);
	desktop.Add(window);

    Button::Ptr saveButton(Button::Create("Save")), cancelButton(Button::Create("Cancel"));
    Button::Ptr mapPathBut(Button::Create("Pick Map Image")), menuPathBut(Button::Create("Pick Menu Image"));
	bool cancelPressed(false), savePressed(false), pickMapImage(false), pickMenuImage(false);
	cancelButton->GetSignal(Button::OnLeftClick).Connect( [&cancelPressed] { cancelPressed = true; });
	saveButton->GetSignal(Button::OnLeftClick).Connect( [&savePressed] { savePressed = true; });
	mapPathBut->GetSignal(Button::OnLeftClick).Connect( [&pickMapImage] { pickMapImage = true; });
	menuPathBut->GetSignal(Button::OnLeftClick).Connect( [&pickMenuImage] { pickMenuImage = true; });

	Form form;
	form.addField("i", "Id: ",160,(item)?(item->id):(-1));
	form.addField("n", "Name: ",160,(item)?(item->name):(""));
	form.addField("d", "Description: ",220,(item)?(item->description):(""));
	form.addField("v", "Value: ",160,(item)?(item->value):(0));
	form.addField("mp", "Map Image: ",160,(item)?(item->mapImg):(""));
	form.addField("mn", "Menu Image: ",160,(item)?(item->menuImg):(""));
    addItemEffectsToForm(form, (item)?(item->effects):(ItemEffect::List()));
    form.addToParent(winBox);

    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL,5);
    butBox->Pack(mapPathBut,false,false);
    butBox->Pack(menuPathBut,false,false);
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

        if (savePressed) {
			savePressed = false;
			if (item)
				ItemDB::get().removeItem(item->id);
			int id = form.getFieldAsInt("i");
			string name = form.getField("n");
			string desc = form.getField("d");
			int val = form.getFieldAsInt("v");
			string mapImg = form.getField("mp");
			string menuImg = form.getField("mn");
			ItemEffect::List effects = getItemEffectsFromForm(form);
			if (id>0 && name.size()>0 && desc.size()>0 && val>=0 && mapImg.size()>0 && menuImg.size()>0) {
				ItemDB::get().getItems().emplace(id, Item::Ptr(new Item(id,name,desc,effects,val,mapImg,menuImg)));
				updateGui();
				break;
			}
        }
        if (cancelPressed) {
			break;
        }
        if (pickMapImage) { //TODO - use Windows file picker w/ default dir in correct folder? copy image if external
			pickMapImage = false;
			FilePicker picker(desktop, owner, "Map Image", Properties::EntityImagePath, "png");
			if (picker.pickFile())
				form.setField("mp", picker.getChoice());
        }
        if (pickMenuImage) {
			pickMenuImage = false;
			FilePicker picker(desktop, owner, "Menu Image", Properties::MenuImagePath, "png");
			if (picker.pickFile())
				form.setField("mn", picker.getChoice());
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

void ItemDbEditor::update() {
	int editCell = data->getEditCell();
	if (editCell!=-1)
		doItem(editCell);
	if (data->needsReorder())
		data->reorder();
	vector<int> delIds = data->getDeletedIds();
	for (unsigned int i = 0; i<delIds.size(); ++i)
		ItemDB::get().removeItem(delIds[i]);
}

void ItemDbEditor::updateGui() {
	data->removeAll();
	for (map<int,Item::Ptr>::iterator i = ItemDB::get().getItems().begin(); i!=ItemDB::get().getItems().end(); ++i) {
		string desc = i->second->getDescription();
		desc = (desc.size()<60)?(desc):(desc.substr(0,57)+"...");
		vector<string> cols = {
			"Id: "+intToString(i->second->getId()),
			"Name: "+i->second->getName(),
			"Description: "+desc,
			"Value: "+intToString(i->second->getValue())
		};
		Box::Ptr row = packRow(cols);
		data->appendRow(i->first,row);
	}
}
