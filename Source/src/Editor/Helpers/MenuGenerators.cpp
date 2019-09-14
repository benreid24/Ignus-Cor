#include "Editor/Helpers/MenuGenerators.hpp"
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
