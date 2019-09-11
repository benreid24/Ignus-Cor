#ifndef MENUGENERATORS_HPP
#define MENUGENERATORS_HPP

#include "Shared/GUI/Form.hpp"
#include "Shared/Items/ItemEffect.hpp"

void addItemEffectsToForm(Form& form, const ItemEffect::List& effects);
ItemEffect::List getItemEffectsFromForm(Form& form);

//TODO - particle generator presets. more things?

#endif // MENUGENERATORS_HPP
