#ifndef FORM_HPP
#define FORM_HPP

#include <SFGUI/Widgets.hpp>
#include <map>
#include <string>
#include <vector>

/**
 * Helper class to encapsulate Entry's to allow for tabbing between them
 */
class Form {
	sfg::Box::Ptr container;
	sfg::Box::Orientation fillDir;
	std::map<std::string, std::pair<sfg::Label::Ptr,sfg::Entry::Ptr> > fields;
	std::vector<std::string> order;
	std::string curActive;
	bool tabPressedLast;

	/**
	 * Updates the active Entry when tab is pressed
	 */
	void updateActive(std::string caller);

public:
	/**
	 * Creates the form and initializes it with the given direction to populate in
	 */
	Form(sfg::Box::Orientation dir = sfg::Box::Orientation::VERTICAL);

	/**
	 * Adds the field with the given name and width to the form
	 */
	void addField(std::string name, std::string text, int width = 120, std::string value = "");

	/**
	 * Adds the field with the given name and width to the form
	 */
	void addField(std::string name, std::string text, int width, int value);

	/**
	 * Returns the value entered into the given field
	 */
	std::string getField(std::string name);

	/**
	 * Returns the given field as an int
	 */
	int getFieldAsInt(std::string name);

	/**
	 * Sets the value of the given field
	 */
	void setField(std::string name, std::string val);

	/**
	 * Adds the form GUI elements to the given parent
	 */
	void addToParent(sfg::Box::Ptr parent);

	/**
	 * Removes the form from the parent
	 */
	void removeFromParent(sfg::Box::Ptr parent);

	/**
	 * Call once per update loop
	 */
	void update();
};

#endif // FORM_HPP
