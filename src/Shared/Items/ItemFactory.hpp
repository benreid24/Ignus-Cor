#ifndef ITEMFACTORY_HPP
#define ITEMFACTORY_HPP

#include "Shared/Items/Item.hpp"
#include "Shared/Util/File.hpp"
#include <map>

/**
 * Factory class to load items from a file
 *
 * \ingroup Items
 */
class ItemFactory {
public:
    /**
     * Loads the items and returns them in a map
     */
    static std::map<int, Item::Ptr> getItems(File& file);
};

#endif // ITEMFACTORY_HPP
