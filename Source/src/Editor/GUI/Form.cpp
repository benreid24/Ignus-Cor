#include "Editor/GUI/Form.hpp"
#include "Shared/Util/Util.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <algorithm>
using namespace sfg;
using namespace std;
using namespace sf;

Form::Form(Box::Orientation dir) {
	container = Box::Create(dir);
	fillDir = dir;
	tabPressedLast = false;
}

void Form::pack(const string& key, Widget::Ptr w) {
    keyWidgetMap.emplace(key, w);
    container->Pack(w,false,false);
}

void Form::hideElements(const string& key) {
    auto iters = keyWidgetMap.equal_range(key);
    for (auto i = iters.first; i!=iters.second && i!=keyWidgetMap.end(); ++i) {
        i->second->Show(false);
    }
}

void Form::showElements(const string& key) {
    auto iters = keyWidgetMap.equal_range(key);
    for (auto i = iters.first; i!=iters.second && i!=keyWidgetMap.end(); ++i) {
        i->second->Show(true);
    }
}

void Form::addField(const string& name, string text, int width, string value) {
	Form* me = this;
	order.push_back(name);
	Box::Orientation rowDir = getAntiFillDir();
	Box::Ptr line = Box::Create(rowDir,5);
	Label::Ptr lbl = Label::Create(text);
	Entry::Ptr entry = Entry::Create(value);
	entry->SetRequisition(Vector2f(width,20));
	entry->GetSignal(Entry::OnGainFocus).Connect( [me,name] { me->updateActive(name); });
	line->Pack(lbl,false,false);
	line->Pack(entry,false,false);
	pack(name, line);
	fields[name] = make_pair(lbl,entry);
	if (fields.size()==1) {
		curActive = name;
		entry->GrabFocus();
	}
}

void Form::addField(const string& name, string text, int width, double value) {
	addField(name, text, width, intToString(value));
}

void Form::updateActive(string caller) {
	curActive = caller;
}

string Form::getField(const string& name) {
	return fields[name].second->GetText();
}

int Form::getFieldAsInt(const string& name) {
	return stringToInt(getField(name));
}

double Form::getFieldAsDouble(const string& name) {
	return stringToInt(getField(name));
}

void Form::setField(const string& name, string val) {
	fields[name].second->SetText(val);
}

void Form::setField(const string& name, double val) {
	fields[name].second->SetText(intToString(val));
}

void Form::addSubform(const string& name, Form& form) {
    Form& f = subforms.emplace(name, form).first->second;
    pack(name, Separator::Create(Separator::Orientation(getAntiFillDir())));
    pack(name, f.container);
    pack(name, Separator::Create(Separator::Orientation(getAntiFillDir())));
}

Form& Form::getSubform(const string& name) {
    auto i = subforms.find(name);
    if (subforms.end() == i)
        return *this;
    return i->second;
}

void Form::hideInput(const string& name) {
    hideElements(name);
}

void Form::showInput(const string& name) {
    showElements(name);
}

void Form::addDropdown(const string& name, const string& desc, const vector<string>& opts, ComboCb cb) {
    ComboBox::Ptr box = ComboBox::Create();
    for (auto i = opts.begin(); i!=opts.end(); ++i) {
        box->AppendItem(*i);
    }
    ComboBox* b = box.get();
    if (cb)
        box->GetSignal(ComboBox::OnSelect).Connect( [b, cb] { cb(b->GetSelectedItem()); } );
    Box::Ptr row = Box::Create(getAntiFillDir());
    row->Pack(Label::Create(desc),false,false);
    row->Pack(box,false,false);
    pack(name, row);
    dropdowns[name] = box;
}

int Form::getSelectedDropdownOption(const string& name) const {
    auto i = dropdowns.find(name);
    if (i != dropdowns.end())
        return i->second->GetSelectedItem();
    return -1;
}

void Form::setDropdownSelection(const string& name, int i) {
    auto d = dropdowns.find(name);
    if (d != dropdowns.end())
        d->second->SelectItem(i);
}

Box::Orientation Form::getAntiFillDir() {
    return (fillDir==Box::Orientation::HORIZONTAL)?(Box::Orientation::VERTICAL):(Box::Orientation::HORIZONTAL);
}

Box::Orientation Form::getFillDir() {
    return fillDir;
}

void Form::addToParent(Box::Ptr parent) {
	parent->Pack(container,false,false);
}

void Form::removeFromParent(Box::Ptr parent) {
	parent->Remove(container);
}

void Form::update() {
	if (sf::Keyboard::isKeyPressed(Keyboard::Tab))
		tabPressedLast = true;
	else if (tabPressedLast) {
		tabPressedLast = false;
		auto i = find(order.begin(),order.end(),curActive);
		if (i!=order.end())
			++i;
		if (i!=order.end()) {
            curActive = *i;
            fields[*i].second->GrabFocus();
		}
	}
}
