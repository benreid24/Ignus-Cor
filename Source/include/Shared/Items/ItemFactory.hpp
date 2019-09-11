#ifndef ITEMFACTORY_HPP
#define ITEMFACTORY_HPP

#include "Shared/Items/Item.hpp"
#include "Shared/Util/File.hpp"
#include <map>

class ItemFactory {
public:
    static std::map<int, Item::Ptr> getItems(File& file);
};

#endif // ITEMFACTORY_HPP
