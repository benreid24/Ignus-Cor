#include "Shared/Items/ItemFactory.hpp"
#include "Shared/Combat/CombatArmor.hpp"
#include "Shared/Combat/CombatAttack.hpp"
using namespace std;

map<int, Item::Ptr> ItemFactory::getItems(File& file) {
    map<int, Item::Ptr> items;

    int count = file.get<uint16_t>();
	for (int i = 0; i<count; ++i) {
        int id = file.get<uint16_t>();
        Item::Category cat = (Item::Category)file.get<uint8_t>();
        string name = file.getString();

        ItemEffect::List effects;
        int nEffects = file.get<uint8_t>();
        for (int j = 0; j<nEffects; ++j) {
            ItemEffect::Type t = ItemEffect::Type(file.get<uint16_t>());
            int intensity = file.get<uint32_t>();
            int ms = file.get<uint32_t>();
            float odds = float(file.get<uint16_t>()) / 1000.0;
            effects.emplace_back(t, intensity, ms);
        }

        int value = file.get<uint32_t>();
        string desc = file.getString();
        string mp = file.getString();
        string mn = file.getString();

        if (cat == Item::Armor) {
            double dr = double(file.get<uint32_t>()) / 100.0;
            items.emplace(id, Item::Ptr(new CombatArmor(id,name,desc,effects,value,mp,mn,dr)));
        }
        else if (cat == Item::Weapon || cat == Item::SpellTomb) {
            CombatAttack::Type type = CombatAttack::Type(file.get<uint16_t>());
            double power = file.get<uint32_t>();
            double delaySeconds = double(file.get<uint16_t>()) / 100.0;
            string animation = file.getString();
            auto particleGenerator = ParticleGeneratorFactory::Preset(file.get<uint8_t>());
            double particlePersistTime = double(file.get<uint16_t>()) / 100.0;

            if (type == CombatAttack::Ranged) {
                string impactAnimation = file.getString();
                double speed = file.get<uint16_t>();
                double range = file.get<uint16_t>();
                auto impactParticleGenerator = ParticleGeneratorFactory::Preset(file.get<uint8_t>());
                double impactParticlePersistTime = double(file.get<uint16_t>()) / 100.0;

                items.emplace(id, Item::Ptr(new CombatAttack(id, name, desc, effects, value,
                                                             mp, mn, power, delaySeconds,
                                                             animation, particleGenerator,
                                                             particlePersistTime, cat,
                                                             range, speed, impactAnimation,
                                                             impactParticleGenerator,
                                                             impactParticlePersistTime)));
            }
            else {
                items.emplace(id, Item::Ptr(new CombatAttack(id, name, desc, effects, value,
                                                             mp, mn, power, delaySeconds,
                                                             animation, particleGenerator,
                                                             particlePersistTime)));
            }
        }
        else {
            items.emplace(id, Item::Ptr(new Item(id,cat,name,desc,effects,value,mp,mn)));
        }
	}

	items[Item::DefaultArmor] = Item::Ptr(
        new CombatArmor(
            Item::DefaultArmor,
            "Clothes",
            "Default armor. Provides zero protection. Should not be visible to player",
            ItemEffect::List(), 0, "", "", 0));

    items[Item::DefaultWeapon] = CombatAttack::Ptr(
        new CombatAttack(Item::DefaultWeapon,
                         "TestSword",
                         "This is for testing",
                         ItemEffect::List(),
                         0, "", "",
                         30,
                         0.6,
                         "Combat/Weapons/TestSword",
                         ParticleGeneratorFactory::None)
    );
	items[Item::DefaultSpell] = CombatAttack::Ptr(
         new CombatAttack(Item::DefaultSpell,
                          "TestSpell",
                          "This is for testing",
                          ItemEffect::List(),
                          0, "", "",
                          30,
                          0.4,
                          "Combat/Spells/fireball.anim",
                          ParticleGeneratorFactory::Smoke,
                          0,
                          Item::SpellTomb,
                          320,
                          400,
                          "Combat/Explosions/testexplosion.anim",
                          ParticleGeneratorFactory::Smoke, 0.5)
    );

    return items;
}
