#ifndef COMBATARMOR_HPP
#define COMBATARMOR_HPP

#include "Shared/Items/Item.hpp"

/**
 * Armor that Entities can wear to help negate combat damage
 *
 * \ingroup Combat
 */
class CombatArmor : public Item {
    //TODO - graphics change?
    double damageResist;

    CombatArmor() = delete;

public:
    typedef std::shared_ptr<const CombatArmor> Ref;

    /**
     * Helper function for pointer cast
     */
    static Ref fromItem(Item::ConstPtr item) { return std::dynamic_pointer_cast<const CombatArmor, const Item>(item); }

    /**
     * Constructs from all parameters
     */
    CombatArmor(int id, const std::string& name, const std::string& desc,
                const ItemEffect::List& effects, int value, const std::string& mapImg,
                const std::string& menuImg, double damageResistance);

    /**
     * Returns the damage resistance
     */
    double getDamageResist() const;
};

#endif // COMBATARMOR_HPP
