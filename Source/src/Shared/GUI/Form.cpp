#include "Shared/GUI/Form.hpp"
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

void Form::addField(string name, string text, int width, string value) {
	Form* me = this;
	order.push_back(name);
	Box::Orientation rowDir = (fillDir==Box::Orientation::HORIZONTAL)?(Box::Orientation::VERTICAL):(Box::Orientation::HORIZONTAL);
	Box::Ptr line = Box::Create(rowDir,5);
	Label::Ptr lbl = Label::Create(text);
	Entry::Ptr entry = Entry::Create(value);
	entry->SetRequisition(Vector2f(width,20));
	entry->GetSignal(Entry::OnGainFocus).Connect( [me,name] { me->updateActive(name); });
	line->Pack(lbl,false,false);
	line->Pack(entry,false,false);
	container->Pack(line,false,false);
	fields[name] = make_pair(lbl,entry);
	if (fields.size()==1) {
		curActive = name;
		entry->GrabFocus();
	}
}

void Form::updateActive(string caller) {
	curActive = caller;
}

string Form::getField(string name) {
	return fields[name].second->GetText();
}

int Form::getFieldAsInt(string name) {
	return stringToInt(getField(name));
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
