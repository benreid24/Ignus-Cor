#include <SFGUI/SFGUI.hpp>
#include "Editor/ItemDbEditor.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Items/ItemDB.hpp"
#include "Shared/Combat/CombatArmor.hpp"
#include "Shared/Combat/CombatAttack.hpp"
#include "Editor/GUI/Form.hpp"
#include "Editor/Helpers/FileImporter.hpp"
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
    if (id < 0)
        return;

    Item::ConstPtr item = ItemDB::get().getItem(id);

    owner->SetState(Widget::State::INSENSITIVE);
    sfg::Window::Ptr window = sfg::Window::Create();
    window->SetTitle("Item");
    Box::Ptr winBox = Box::Create(Box::Orientation::VERTICAL,5);
    window->Add(winBox);
    desktop.Add(window);

    Button::Ptr saveButton(Button::Create("Save")), cancelButton(Button::Create("Cancel"));
    Button::Ptr mapPathBut(Button::Create("Pick Map Image")), menuPathBut(Button::Create("Pick Menu Image"));
    Button::Ptr impactBut(Button::Create("Pick Impact Animation")), mapAnimBut(Button::Create("Pick MapAnimation"));
    bool cancelPressed(false), savePressed(false), pickMapImage(false), pickMenuImage(false), impactAnimPressed(false), mapAnimPressed(false);
    cancelButton->GetSignal(Button::OnLeftClick).Connect( [&cancelPressed] { cancelPressed = true; });
    saveButton->GetSignal(Button::OnLeftClick).Connect( [&savePressed] { savePressed = true; });
    mapPathBut->GetSignal(Button::OnLeftClick).Connect( [&pickMapImage] { pickMapImage = true; });
    menuPathBut->GetSignal(Button::OnLeftClick).Connect( [&pickMenuImage] { pickMenuImage = true; });
    impactBut->GetSignal(Button::OnLeftClick).Connect( [&impactAnimPressed] { impactAnimPressed = true; });
    mapAnimBut->GetSignal(Button::OnLeftClick).Connect( [&mapAnimPressed] { mapAnimPressed = true; });

    int lastSelectedCat = -1;

    Form form;
    form.addField("i", "Id: ",160,(item)?(item->id):(ItemDB::get().getNextId()));
    form.addDropdown("cat", "Type: ", Item::getAllCategories());
    if (item) form.setDropdownSelection("cat", item->getCategory());
    form.addField("n", "Name: ",160,(item)?(item->name):(""));
    form.addField("d", "Description: ",220,(item)?(item->description):(""));
    form.addField("v", "Value: ",160,(item)?(item->value):(0));
    form.addField("mp", "Map Image/Anim: ",160,(item)?(item->mapGfx):(""));
    form.addField("mn", "Menu Image: ",160,(item)?(item->menuImg):(""));

    Form armorForm;
    armorForm.addField("dr", "Damage Resistance: ", 160);
    form.addSubform("armor", armorForm);
    if (item && item->getCategory() == Item::Armor) {
        CombatArmor::Ref a = CombatArmor::fromItem(item);
        armorForm.setField("dr", a->getDamageResist());
    }
    else
        form.hideInput("armor");

    Form weaponForm;
    weaponForm.addDropdown("type", "Type: ", {"Melee", "Ranged"});
    weaponForm.setDropdownSelection("type", 0);
    weaponForm.addField("pwr", "Power: ");
    weaponForm.addField("cldn", "Cooldown(s): ", 160);
    addParticleGeneratorsToForm(weaponForm, "Particle Generator");
    weaponForm.addField("ptm", "Particle Gen Time(s): ");
    form.addSubform("weapon", weaponForm);

    Form rangedForm;
    rangedForm.addField("colani", "Impact Animation: ", 240);
    rangedForm.addField("rng", "Range(px): ");
    rangedForm.addField("spd", "Speed(px/s): ");
    addParticleGeneratorsToForm(rangedForm, "Impact Particle Generator");
    rangedForm.addField("ptm", "Impact Particle Gen Time(s): ");
    form.addSubform("ranged", rangedForm);

    if (item && (item->getCategory() == Item::Weapon || item->getCategory() == Item::SpellTomb)) {
        CombatAttack::Ref a = CombatAttack::fromItem(item);
        weaponForm.setDropdownSelection("type", a->getType()==CombatAttack::Melee ? 0 : 1);
        weaponForm.setField("pwr", a->getPower());
        weaponForm.setField("cldn", a->getAttackDelay());
        weaponForm.setDropdownSelection("Particle Generator", a->getParticleType());
        weaponForm.setField("ptm", a->getParticlePersistanceTime());
        rangedForm.setField("colani", a->getImpactAnimation());
        rangedForm.setField("rng", a->getRange());
        rangedForm.setField("spd", a->getSpeed());
        rangedForm.setDropdownSelection("Impact Particle Generator", a->getImpactParticles());
        rangedForm.setField("ptm", a->getImpactParticlePersistance());
    }
    else {
        form.hideInput("weapon");
        form.hideInput("ranged");
    }

    list<Form> effectForms = addItemEffectsToForm(form, (item)?(item->effects):(ItemEffect::List()));

    Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL,5);
    butBox->Pack(mapPathBut,false,false);
    butBox->Pack(menuPathBut,false,false);
    winBox->Pack(butBox,false,false);

    butBox = Box::Create(Box::Orientation::HORIZONTAL,5);
    butBox->Pack(mapAnimBut,false,false);
    butBox->Pack(impactBut,false,false);
    winBox->Pack(butBox,false,false);
    impactBut->Show(false);

    form.addToParent(winBox);
    butBox = Box::Create(Box::Orientation::HORIZONTAL,5);
    butBox->Pack(saveButton,true,true);
    butBox->Pack(cancelButton,true,true);
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
                if (weaponForm.getSelectedDropdownOption("type") == 1) {
                    form.showInput("ranged");
                    impactBut->Show(true);
                }
                else {
                    form.hideInput("ranged");
                    impactBut->Show(false);
                }
            }
            else {
                form.hideInput("weapon");
                form.hideInput("ranged");
                impactBut->Show(false);
            }
        }
        if (lastSelectedCat == Item::Weapon || lastSelectedCat == Item::SpellTomb) {
            if (weaponForm.getSelectedDropdownOption("type") == 1) {
                form.showInput("ranged");
                impactBut->Show(true);
            }
            else {
                form.hideInput("ranged");
                impactBut->Show(false);
            }
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
            ItemEffect::List effects = getItemEffectsFromForm(effectForms);
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
                    ParticleGeneratorFactory::Preset pgen = getParticleGeneratorFromForm(weaponForm, "Impact Particle");
                    double ptime = weaponForm.getFieldAsDouble("ptm");
                    if (weaponForm.getSelectedDropdownOption("type") == 0)
                        ItemDB::get().getItems().emplace(id, Item::Ptr(new CombatAttack(id,name,desc,effects,val,mapImg,menuImg,pwr,cldn,pgen,ptime)));
                    else {
                        string ianim = rangedForm.getField("colani");
                        double rng = rangedForm.getFieldAsDouble("rng");
                        double spd = rangedForm.getFieldAsDouble("spd");
                        ParticleGeneratorFactory::Preset ipgen = getParticleGeneratorFromForm(rangedForm, "Impact Particle Generator");
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
        if (pickMapImage) {
            pickMapImage = false;
            FileImporter importer(Properties::EntityImagePath, FileImporter::Image);
            string file = importer.importFile(desktop, window);
            if (!file.empty())
                form.setField("mp", file);
        }
        if (mapAnimPressed) {
            mapAnimPressed = false;
            FileImporter importer(Properties::ItemAnimationPath, FileImporter::Animation);
            string file = importer.importFile(desktop, window);
            if (!file.empty())
                form.setField("mp", file);
        }
        if (pickMenuImage) {
            pickMenuImage = false;
            FileImporter importer(Properties::ItemMenuImagePath, FileImporter::Image);
            string file = importer.importFile(desktop, window);
            if (!file.empty())
                form.setField("mn", file);
        }
        if (impactAnimPressed) {
            impactAnimPressed = false;
            FileImporter importer(Properties::ItemAnimationPath, FileImporter::Animation);
            string file = importer.importFile(desktop, window);
            if (!file.empty())
                rangedForm.setField("colani", file);
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
