#include <SFGUI/SFGUI.hpp>
#include "Editor/ItemDbEditor.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Items/ItemDB.hpp"
#include "Shared/Combat/CombatArmor.hpp"
#include "Shared/Combat/CombatAttack.hpp"
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

	container = Box::Create(Box::Orientation::VERTICAL,4);
	container->SetRequisition(Vector2f(Properties::ScreenWidth-50,Properties::ScreenHeight-50));

	Button::Ptr saveBut = Button::Create("Save"), appBut = Button::Create("New");
    saveBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->save(); });
    appBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->doItem(); });
    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL, 5);
    butBox->Pack(saveBut,false,false);
    butBox->Pack(appBut,false,false);
    container->Pack(butBox,false,false);

    cats = ComboBox::Create();
    cats->AppendItem("All");
    for (auto i = Item::getAllCategories().begin(); i!=Item::getAllCategories().end(); ++i) {
        cats->AppendItem(*i);
    }
    cats->SelectItem(0);
    cats->GetSignal(ComboBox::OnSelect).Connect( [me] { me->updateGui(); } );
    Label::Ptr catLbl = Label::Create("Filter by type:");
    Box::Ptr catBox = Box::Create(Box::Orientation::HORIZONTAL);
    catBox->Pack(catLbl,false,false);
    catBox->Pack(cats,false,false);
    container->Pack(catBox,false,false);

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

void ItemDbEditor::update() {
	int editCell = data->getEditCell();
	if (editCell!=-1)
		doItem(editCell);
	if (data->needsReorder())
		data->reorder();
	vector<int> delIds = data->getDeletedIds();
	for (unsigned int i = 0; i<delIds.size(); ++i) {
		if (delIds[i] >= 0)
            ItemDB::get().removeItem(delIds[i]);
	}
}

void ItemDbEditor::updateGui() {
	data->removeAll();
	int cat = cats->GetSelectedItem() - 1;
	for (map<int,Item::Ptr>::iterator i = ItemDB::get().getItems().begin(); i!=ItemDB::get().getItems().end(); ++i) {
		if (int(i->second->getCategory()) != cat && cat!=-1)
            continue;

		string desc = i->second->getDescription();
		desc = (desc.size()<60)?(desc):(desc.substr(0,57)+"...");
		vector<string> cols = {
			"Id: "+intToString(i->second->getId()),
			"Type: "+Item::getCategorySingular(i->second->getCategory()),
			"Name: "+i->second->getName(),
			"Description: "+desc,
			"Value: "+intToString(i->second->getValue())
		};
		Box::Ptr row = packRow(cols);
		data->appendRow(i->first,row);
	}
}

void ItemDbEditor::doItem(int id) {
   // if (id < 0)
   //     return;

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

	int lastSelectedCat = -1;

	Form form;
	form.addField("i", "Id: ",160,(item)?(item->id):(0));
	form.addDropdown("cat", "Type: ", Item::getAllCategories());
	if (item) form.setDropdownSelection("cat", item->getCategory());
	form.addField("n", "Name: ",160,(item)?(item->name):(""));
	form.addField("d", "Description: ",220,(item)?(item->description):(""));
	form.addField("v", "Value: ",160,(item)?(item->value):(0));
	form.addField("mp", "Map Image: ",160,(item)?(item->mapImg):(""));
	form.addField("mn", "Menu Image: ",160,(item)?(item->menuImg):(""));

	Form armorForm;
	armorForm.addField("dr", "Damage Resistance: ", 160);
	form.addSubform("armor", armorForm);
	if (item && item->getCategory() == Item::Armor) {
        CombatArmor::Ref a = CombatArmor::fromItem(item);
        armorForm.setField("dr", intToString(a->getDamageResist()));
	}
	else
        form.hideInput("armor");

	Form weaponForm;
	weaponForm.addDropdown("type", "Type: ", {"Melee", "Ranged"});
	weaponForm.addField("pwr", "Power: ");
	weaponForm.addField("cldn", "Cooldown: ", 160);
	weaponForm.addField("pts", "Particle Generator: ");
	weaponForm.addField("ptm", "Particle Gen Time: ");
	form.addSubform("weapon", weaponForm);

	Form rangedForm;
	rangedForm.addField("colani", "Impact Animation: ", 240);
	rangedForm.addField("rng", "Range: ");
	rangedForm.addField("spd", "Speed: ");
	rangedForm.addField("pts", "Impact Particle Generator: ");
	rangedForm.addField("ptm", "Impact Particle Gen Time: ");
	form.addSubform("ranged", rangedForm);

	if (item && (item->getCategory() == Item::Weapon || item->getCategory() == Item::SpellTomb)) {
        CombatAttack::Ref a = CombatAttack::fromItem(item);
        weaponForm.setDropdownSelection("type", a->getType()==CombatAttack::Melee ? 0 : 1);
        weaponForm.setField("pwr", a->getPower());
        weaponForm.setField("cldn", a->getAttackDelay());
        weaponForm.setField("pts", a->getParticleType());
        weaponForm.setField("ptm", a->getParticlePersistanceTime());
        rangedForm.setField("colani", a->getImpactAnimation());
        rangedForm.setField("rng", a->getRange());
        rangedForm.setField("spd", a->getSpeed());
        rangedForm.setField("pts", a->getImpactParticles());
        rangedForm.setField("ptm", a->getImpactParticlePersistance());
    }
    else {
        form.hideInput("weapon");
        form.hideInput("ranged");
    }

    //addItemEffectsToForm(form, (item)?(item->effects):(ItemEffect::List()));

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

        if (form.getSelectedDropdownOption("cat") != lastSelectedCat) {
            lastSelectedCat = form.getSelectedDropdownOption("cat");
            Item::Category c = Item::Category(lastSelectedCat);

            if (c == Item::Armor) {
                form.showInput("armor");
            }
            else
                form.hideInput("armor");

            if (c == Item::Weapon || c == Item::SpellTomb) {
                form.showInput("weapon");
                if (weaponForm.getSelectedDropdownOption("type") == 1)
                    form.showInput("ranged");
                else
                    form.hideInput("ranged");
            }
            else {
                form.hideInput("weapon");
                form.hideInput("ranged");
            }
        }
        if (lastSelectedCat == Item::Weapon || lastSelectedCat == Item::SpellTomb) {
            if (weaponForm.getSelectedDropdownOption("type") == 1)
                form.showInput("ranged");
            else
                form.hideInput("ranged");
        }

        if (savePressed) {
			savePressed = false;

			int id = form.getFieldAsInt("i");
			Item::Category cat = Item::Category(form.getSelectedDropdownOption("cat"));
			string name = form.getField("n");
			string desc = form.getField("d");
			int val = form.getFieldAsInt("v");
			string mapImg = form.getField("mp");
			string menuImg = form.getField("mn");
			ItemEffect::List effects = ItemEffect::List();
			if (id>0 && name.size()>0 && desc.size()>0 && val>=0 && mapImg.size()>0 && menuImg.size()>0) {
                if (item)
                    ItemDB::get().removeItem(item->id);
                if (cat == Item::Armor) {
                    double dr = armorForm.getFieldAsDouble("dr");
                    ItemDB::get().getItems().emplace(id, Item::Ptr(new CombatArmor(id,name,desc,effects,val,mapImg,menuImg,dr)));
                }
                else if (cat == Item::Weapon || cat == Item::SpellTomb) {
                    double pwr = weaponForm.getFieldAsDouble("pwr");
                    double cldn = weaponForm.getFieldAsDouble("cldn");
                    ParticleGeneratorFactory::Preset pgen = ParticleGeneratorFactory::Preset(weaponForm.getFieldAsInt("pts"));
                    double ptime = weaponForm.getFieldAsDouble("ptm");
                    if (weaponForm.getSelectedDropdownOption("type") == 0)
                        ItemDB::get().getItems().emplace(id, Item::Ptr(new CombatAttack(id,name,desc,effects,val,mapImg,menuImg,pwr,cldn,pgen,ptime)));
                    else {
                        string ianim = rangedForm.getField("colani");
                        double rng = rangedForm.getFieldAsDouble("rng");
                        double spd = rangedForm.getFieldAsDouble("spd");
                        ParticleGeneratorFactory::Preset ipgen = ParticleGeneratorFactory::Preset(rangedForm.getFieldAsInt("pts"));
                        double iptime = rangedForm.getFieldAsDouble("ptm");
                        ItemDB::get().getItems().emplace(id, Item::Ptr(new CombatAttack(id,name,desc,effects,val,mapImg,menuImg,pwr,cldn,pgen,ptime,cat,rng,spd,ianim,ipgen,iptime)));
                    }
                }
                else
                    ItemDB::get().getItems().emplace(id, Item::Ptr(new Item(id,cat,name,desc,effects,val,mapImg,menuImg)));
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
