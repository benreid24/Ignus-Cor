#include "Editor/Helpers/MenuGenerators.hpp"
#include "Shared/Util/Util.hpp"
using namespace std;

namespace {

const string ITEMEFFECTS = "itemeffectssubform";

const ItemEffect* getEffectFromList(const ItemEffect::List& effects, const string& desc) {
    for (auto i = effects.begin(); i!=effects.end(); ++i) {
        if (i->getName() == desc)
            return &(*i);
    }
    return nullptr;
}

}

void addItemEffectsToForm(Form& form, const ItemEffect::List& effects) {
    const vector<pair<string,string> >& effectList = ItemEffect::getAllEffects();
    Form sf(form.getFillDir());

    for (unsigned int i = 1; i<effectList.size(); ++i) {
        const ItemEffect* e = getEffectFromList(effects, effectList[i].first);
        const string val = (e)?(intToString(e->getIntensity())):("");
        sf.addField(effectList[i].first, effectList[i].first, 60, val);
    }
    form.addSubform(ITEMEFFECTS, sf);
}

ItemEffect::List getItemEffectsFromForm(Form& form) {
    const vector<pair<string,string> >& effectList = ItemEffect::getAllEffects();
    Form& sf = form.getSubform(ITEMEFFECTS);
    ItemEffect::List effects;

    for (unsigned int i = 1; i<effectList.size(); ++i) {
        string val = sf.getField(effectList[i].first);
        if (!val.empty()) {
            effects.push_back(ItemEffect(effectList[i].first, stringToInt(val)));
        }
    }

    return effects;
}
