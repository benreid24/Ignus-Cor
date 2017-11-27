#include <SFGUI/SFGUI.hpp>
#include "Editor/ItemDbEditor.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Data/ItemDB.hpp"
#include "Shared/GUI/Form.hpp"
#include "Editor/Helpers/FilePicker.hpp"
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
	ItemDB::save();
}

void ItemDbEditor::doItem(int id) {
	Item* item = ItemDB::itemExists(id)?(&ItemDB::getItem(id)):(nullptr);

	owner->SetState(Widget::State::INSENSITIVE);
    sfg::Window::Ptr window = sfg::Window::Create();
    window->SetTitle("Item");
    Box::Ptr winBox = Box::Create(Box::Orientation::VERTICAL,5);
	window->Add(winBox);
	desktop.Add(window);

    Form form;
    Button::Ptr saveButton(Button::Create("Save")), cancelButton(Button::Create("Cancel"));
	bool cancelPressed(false), savePressed(false), pickPressed(false);
	cancelButton->GetSignal(Button::OnLeftClick).Connect( [&cancelPressed] { cancelPressed = true; });
	saveButton->GetSignal(Button::OnLeftClick).Connect( [&savePressed] { savePressed = true; });

	form.addField("i", "Id: ",160,(item!=nullptr)?(item->id):(-1));
	form.addField("n", "Name: ",160,(item!=nullptr)?(item->name):(""));
	form.addField("d", "Description: ",220,(item!=nullptr)?(item->description):(""));
	form.addField("v", "Value: ",160,(item!=nullptr)?(item->value):(0));
	form.addField("mp", "Map Image: ",160,(item!=nullptr)?(item->mapImg):(""));
	form.addField("mn", "Menu Image: ",160,(item!=nullptr)?(item->menuImg):(""));
	form.addField("int", "Effect Intensity: ",80, (item!=nullptr)?(item->intensity):(0));
    form.addToParent(winBox);

    Box::Ptr box = Box::Create(Box::Orientation::HORIZONTAL,5);
    ComboBox::Ptr effectEntry = ComboBox::Create();
    for (unsigned int i = 0; i<Effects::effectStrings.size(); ++i)
		effectEntry->AppendItem(Effects::effectStrings[i]);
    effectEntry->SelectItem((item!=nullptr)?(item->effect):(0));
    box->Pack(Label::Create("Effect: "),false,false);
    box->Pack(effectEntry,false,false);
    winBox->Pack(box,false,false);

    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL,5);
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
			if (item!=nullptr)
				ItemDB::removeItem(item->id);
			int id = form.getFieldAsInt("i");
			string name = form.getField("n");
			string desc = form.getField("d");
			int val = form.getFieldAsInt("v");
			int intense = form.getFieldAsInt("int");
			string mapImg = form.getField("mp");
			string menuImg = form.getField("mn");
			Effects::Effect effect = Effects::Effect(Effects::effectMap[effectEntry->GetSelectedItem()]);
			ItemDB::getItems()[id] = new Item(id,name,desc,effect,intense,val,mapImg,menuImg);
			updateGui();
			break;
        }
        if (cancelPressed) {
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

void ItemDbEditor::update() {
	int editCell = data->getEditCell();
	if (editCell!=-1)
		doItem(editCell);
	if (data->needsReorder())
		data->reorder();
}

void ItemDbEditor::updateGui() {
    vector<int> ids = data->getIds();
    for (unsigned int i = 0; i<ids.size(); ++i)
		data->removeRow(ids[i]);
	for (map<int,Item*>::iterator i = ItemDB::getItems().begin(); i!=ItemDB::getItems().end(); ++i) {
		string desc = i->second->getDescription();
		desc = (desc.size()<60)?(desc):(desc.substr(0,57)+"...");
		vector<string> cols = {
			"Id: "+intToString(i->second->getId()),
			"Name: "+i->second->getName(),
			"Description: "+desc,
			"Value: "+intToString(i->second->getValue()),
			"Effect: "+Effects::getEffectString(i->second->getEffect()),
			"Effect Intensity: "+intToString(i->second->getIntensity())
		};
		Box::Ptr row = packRow(cols);
		data->appendRow(i->first,row);
	}
}
