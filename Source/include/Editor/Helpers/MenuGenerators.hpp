#ifndef MENUGENERATORS_HPP
#define MENUGENERATORS_HPP

#include "Editor/GUI/Form.hpp"
#include "Shared/Items/ItemEffect.hpp"
#include <list>

std::list<Form> addItemEffectsToForm(Form& form, const ItemEffect::List& effects);
ItemEffect::List getItemEffectsFromForm(std::list<Form>& effectForms);

//TODO - particle generator presets. more things?

#endif // MENUGENERATORS_HPP
