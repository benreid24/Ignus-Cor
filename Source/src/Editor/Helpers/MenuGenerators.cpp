#include "Editor/Helpers/MenuGenerators.hpp"
#include "Shared/Items/ItemDB.hpp"
#include "Shared/Util/Util.hpp"
using namespace std;

namespace {

const ItemEffect* getEffectFromList(const ItemEffect::List& effects, const string& name) {
    for (auto i = effects.begin(); i!=effects.end(); ++i) {
        if (i->getName() == name)
            return &(*i);
    }
    return nullptr;
}

}

list<Form> addItemEffectsToForm(Form& form, const ItemEffect::List& effects) {
    Form::ComboCb cb = [&form] (int i) {
        form.showInput(ItemEffect(i).getName());
        form.setDropdownSelection("effectadder", 0);
    };
    vector<string> effectNames;
    for (auto i = ItemEffect::getAllEffects().begin(); i!=ItemEffect::getAllEffects().end(); ++i) {
        effectNames.push_back(i->first);
    }
    form.addDropdown("effectadder", "Add Effect: ", effectNames, cb);

    const vector<pair<string,string> >& effectList = ItemEffect::getAllEffects();
    list<Form> forms;

    for (unsigned int i = 1; i<effectList.size(); ++i) {
        string effect = effectList[i].first;
        Form& sf = *(forms.emplace(forms.end(), form.getAntiFillDir()));
        form.addLabel(effect);
        const ItemEffect* e = getEffectFromList(effects, effect);
        string intense = (e)?(intToString(e->getIntensity())):("");
        string ch = (e)?(doubleToString(e->getOdds())):("1");
        string dr = (e)?(doubleToString(e->getDurationMs())):("0");
        sf.addField("int", "Intensity: ", 60, intense);
        sf.addField("c", "Chance[0,1]: ", 60, ch);
        sf.addField("dir", "Duration(ms): ", 60, dr);
        Form::ButtonCb cb = [&form, effect] { form.hideInput(effect); };
        sf.addButton("Remove", cb);
        form.addSubform(effect, sf);
        if (e == nullptr)
            form.hideInput(effect);
    }

    return forms;
}

ItemEffect::List getItemEffectsFromForm(list<Form>& effectForms) {
    const vector<pair<string,string> >& effectList = ItemEffect::getAllEffects();
    ItemEffect::List effects;

    auto f = effectForms.begin();
    for (unsigned int i = 1; i<effectList.size(); ++i, ++f) {
        if (f->isVisible()) {
            int intensity = f->getFieldAsInt("int");
            double odds = f->getFieldAsDouble("c");
            double dur = f->getFieldAsDouble("dir");
            effects.emplace_back(i, intensity, dur, odds);
        }
    }

    return effects;
}

void addParticleGeneratorsToForm(Form& form, const string& name) {
    form.addDropdown(name, name, ParticleGeneratorFactory::getAllPresets());
}

ParticleGeneratorFactory::Preset getParticleGeneratorFromForm(Form& form, const string& name) {
    unsigned int i = form.getSelectedDropdownOption(name);
    if (i >= ParticleGeneratorFactory::getAllPresets().size())
        i = 0;
    return ParticleGeneratorFactory::Preset(i);
}

sfg::ComboBox::Ptr generateItemCategoryFilter() {
    sfg::ComboBox::Ptr box = sfg::ComboBox::Create();
    for (auto i = Item::getAllCategories().begin(); i != Item::getAllCategories().end(); ++i) {
        box->AppendItem(*i);
    }
    return box;
}

Item::Category getItemCategoryFromDropdown(sfg::ComboBox::Ptr box) {
    unsigned int i = box->GetSelectedItem();
    if (i < Item::getAllCategories().size())
        return Item::getCategoryFromName(Item::getAllCategories()[i]);
    return Item::All;
}

sfg::ComboBox::Ptr generateItemSelector(map<int,int>& idMap, map<int,int>& indexMap, Item::Category filter) {
    sfg::ComboBox::Ptr box = sfg::ComboBox::Create();
    int index = 0;
    for (auto i = ItemDB::get().getItems().begin(); i!=ItemDB::get().getItems().end(); ++i, ++index) {
        if (i->second->getCategory() == filter || filter == Item::All) {
            string item = intToString(i->first) + " | ";
            item += i->second->getName() + " | ";
            item += Item::getCategorySingular(i->second->getCategory()) + " | ";
            item += "$" + intToString(i->second->getValue());
            box->AppendItem(item);
            indexMap[i->first] = index;
            idMap[index] = i->first;
        }
    }
    return box;
}
