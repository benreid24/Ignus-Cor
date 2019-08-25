#include "Shared/Data/EffectDB.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Properties.hpp"
using namespace std;

EffectDB::EffectDB() {
    File file(Properties::EffectDbFile, File::In);
    int numEffects = file.get<uint16_t>();
    for (int i = 0; i<numEffects; ++i) {
        CombatEffect effect;
        effect.type = CombatEffect::Type(file.get<uint8_t>());
        effect.name = file.getString();
        effect.description = file.getString();
        effects.emplace(effect.type, effect);
    }
    effects.emplace(0, CombatEffect());
}

EffectDB& EffectDB::get() {
    static EffectDB db;
    return db;
}

CombatEffect::Ref EffectDB::getEffect(int id) {
    auto i = effects.find(id);
    if (effects.end() == i) {
        i = effects.find(0);
        if (effects.end() == i) {
            cout << "FATAL: None CombatEffect has been deleted!\n";
            abort();
        }
    }
    return i->second.makeRef();
}

bool EffectDB::effectExists(int id) {
    return effects.find(id) != effects.end();
}

void EffectDB::removeEffect(int id) {
    if (0 == id) {
        cout << "Warning: Attempted to remove CombatEffect 0 (None)\n";
        return;
    }
    auto i = effects.find(id);
    if (effects.end() != i)
        effects.erase(i);
}

map<int, CombatEffect>& EffectDB::getEffects() {
    return effects;
}

void EffectDB::save() {
    File file(Properties::EffectDbFile, File::Out);
    file.write<uint16_t>(effects.size());
    for (auto i = effects.begin(); i!=effects.end(); ++i) {
        file.write<uint16_t>(i->first);
        file.writeString(i->second.name);
        file.writeString(i->second.description);
    }
}
