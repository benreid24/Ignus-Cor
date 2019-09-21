#ifndef MENUGENERATORS_HPP
#define MENUGENERATORS_HPP

#include "Editor/GUI/Form.hpp"
#include "Shared/Items/Item.hpp"
#include "Shared/Particles/ParticleGeneratorFactory.hpp"
#include <list>

std::list<Form> addItemEffectsToForm(Form& form, const ItemEffect::List& effects);
ItemEffect::List getItemEffectsFromForm(std::list<Form>& effectForms);

void addParticleGeneratorsToForm(Form& form, const std::string& name);
ParticleGeneratorFactory::Preset getParticleGeneratorFromForm(Form& form, const std::string& name);

sfg::ComboBox::Ptr generateItemCategoryFilter();
Item::Category getItemCategoryFromDropdown(sfg::ComboBox::Ptr dropdown);
sfg::ComboBox::Ptr generateItemSelector(std::map<int,int>& indexToIdMap,
                                        std::map<int,int>& idToIndexMap,
                                        Item::Category filter = Item::All);
void integrateItemFilterToList(sfg::ComboBox::Ptr itemFilter, sfg::ComboBox::Ptr itemList,
                               std::map<int,int>* idMap, std::map<int,int>* indexMap);

#endif // MENUGENERATORS_HPP
