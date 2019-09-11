#include "Shared/Combat/CombatArmor.hpp"
using namespace std;

CombatArmor::Ref CombatArmor::fromItem(Item::ConstPtr item) {
    return std::dynamic_pointer_cast<const CombatArmor, const Item>(item);
}

CombatArmor::CombatArmor(int id, const std::string& name, const std::string& desc,
                         const ItemEffect::List& effects, int value, const std::string& mapImg,
                         const std::string& menuImg, double damageResistance)
: Item(id, Item::Armor, name, desc, effects, value, mapImg, menuImg), damageResist(damageResistance) {}

double CombatArmor::getDamageResist() const {
    return damageResist;
}
