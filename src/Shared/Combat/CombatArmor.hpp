#ifndef COMBATARMOR_HPP
#define COMBATARMOR_HPP

#include "Shared/Items/Item.hpp"

class ItemFactory;

/**
 * Armor that Entities can wear to help negate combat damage
 *
 * \ingroup Combat
 * \ingroup Items
 */
class CombatArmor : public Item {
    //TODO - graphics change?
    double damageResist;

    CombatArmor() = delete;

    /**
     * Constructs from all parameters
     */
    CombatArmor(int id, const std::string& name, const std::string& desc,
                const ItemEffect::List& effects, int value, const std::string& mapImg,
                const std::string& menuImg, double damageResistance);

    /**
     * Saves damage resistance to the file
     */
    virtual void p_save(File& file) const override;

    friend class ItemFactory;
    friend class ItemDbEditor;

public:
    typedef std::shared_ptr<const CombatArmor> Ref;

    virtual ~CombatArmor() = default;

    /**
     * Helper function for pointer cast
     */
    static Ref fromItem(Item::ConstPtr item);

    /**
     * Returns the damage resistance
     */
    double getDamageResist() const;
};

#endif // COMBATARMOR_HPP
