#include "Shared/Data/AttackDB.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Properties.hpp"
using namespace std;

AttackDB::AttackDB() {
    File file(Properties::AttackDbFile, File::In);
    int numAttacks = file.get<uint16_t>();
    for (int i = 0; i<numAttacks; ++i) {
        attacks.emplace(file.get<uint16_t>(), make_shared<CombatAttack>(file));
    }

    attacks[-1] = CombatAttack::Ptr(
        new CombatAttack("TestSword",
                         "This is for testing",
                         30,
                         0.6,
                         CombatEffect::List(),
                         "Combat/Weapons/TestSword",
                         ParticleGeneratorFactory::None)
    );
	attacks[-2] = CombatAttack::Ptr(
         new CombatAttack("TestSpell",
                          "This is for testing",
                          30,
                          0.4,
                          CombatEffect::List(),
                          "Combat/Spells/fireball.anim",
                          ParticleGeneratorFactory::Smoke,
                          0,
                          320,
                          400,
                          "Combat/Explosions/testexplosion.anim",
                          ParticleGeneratorFactory::Smoke, 0.5)
    );
}

AttackDB& AttackDB::get() {
    static AttackDB db;
    return db;
}

CombatAttack::ConstPtr AttackDB::getAttack(int id) {
    auto i = attacks.find(id);
    if (attacks.end() != i)
        return i->second;
    return nullptr;
}

bool AttackDB::attackExists(int id) {
    return attacks.find(id) != attacks.end();
}

void AttackDB::removeAttack(int id) {
    auto i = attacks.find(id);
    if (attacks.end() != i)
        attacks.erase(i);
}

map<int, CombatAttack::Ptr>& AttackDB::getAttacks() {
    return attacks;
}

void AttackDB::save() {
    File file(Properties::AttackDbFile, File::Out);
    file.write<uint16_t>(attacks.size());
    for (auto i = attacks.begin(); i != attacks.end(); ++i) {
        file.write<uint16_t>(i->first);
        i->second->save(file);
    }
}
