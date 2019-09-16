#ifndef MENUGENERATORS_HPP
#define MENUGENERATORS_HPP

#include "Editor/GUI/Form.hpp"
#include "Shared/Items/ItemEffect.hpp"
#include "Shared/Particles/ParticleGeneratorFactory.hpp"
#include <list>

std::list<Form> addItemEffectsToForm(Form& form, const ItemEffect::List& effects);
ItemEffect::List getItemEffectsFromForm(std::list<Form>& effectForms);

void addParticleGeneratorsToForm(Form& form, const std::string& name);
ParticleGeneratorFactory::Preset getParticleGeneratorFromForm(Form& form, const std::string& name);

//TODO - Drop downs for items

#endif // MENUGENERATORS_HPP
