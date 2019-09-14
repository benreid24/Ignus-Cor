#ifndef FORM_HPP
#define FORM_HPP

#include <SFGUI/Widgets.hpp>
#include <functional>
#include <map>
#include <string>
#include <vector>

/**
 * Helper class to encapsulate Entry's to allow for tabbing between them
 */
class Form {
	sfg::Box::Ptr container;
	sfg::Box::Orientation fillDir;
	std::multimap<std::string,sfg::Widget::Ptr> keyWidgetMap;
	std::map<std::string, std::pair<sfg::Label::Ptr,sfg::Entry::Ptr> > fields;
	std::map<std::string, Form&> subforms;
	std::map<std::string, sfg::ComboBox::Ptr> dropdowns;
	std::vector<std::string> order;
	std::string curActive;
	bool tabPressedLast;

	/**
	 * Updates the active Entry when tab is pressed
	 */
	void updateActive(std::string caller);

	/**
	 * Packs a widget with the given key
	 */
    void pack(const std::string& key, sfg::Widget::Ptr widget);

    /**
     * Hides all elements relating to the given key
     */
    void hideElements(const std::string& key);

    /**
     * Shows all elements relating to the given key
     */
    void showElements(const std::string& key);

public:
    typedef std::function<void(int)> ComboCb;

	/**
	 * Creates the form and initializes it with the given direction to populate in
	 */
	Form(sfg::Box::Orientation dir = sfg::Box::Orientation::VERTICAL);

	/**
	 * Adds the field with the given name and width to the form
	 */
	void addField(const std::string& name, std::string text, int width = 120, std::string value = "");

	/**
	 * Adds the field with the given name and width to the form
	 */
	void addField(const std::string& name, std::string text, int width, double value);

	/**
	 * Returns the value entered into the given field
	 */
	std::string getField(const std::string& name);

	/**
	 * Returns the given field as an int
	 */
	int getFieldAsInt(const std::string& name);
	/**
	 * Returns the given field as a double
	 */
	double getFieldAsDouble(const std::string& name);

	/**
	 * Sets the value of the given field
	 */
	void setField(const std::string& name, std::string val);

	/**
	 * Helper function to set a field value from a double
	 */
    void setField(const std::string& name, double val);

	/**
	 * Adds the subform to this form
	 */
    void addSubform(const std::string& name, Form& form);

    /**
     * Returns a reference to the requested subform, or a reference to this form if it does not exist
     */
    Form& getSubform(const std::string& name);

    /**
     * Hide the input from the form
     */
    void hideInput(const std::string& name);

    /**
     * Shows the input from the form
     */
    void showInput(const std::string& name);

    /**
     * Adds a drop down with the given options and optional callback for when the value changes
     */
    void addDropdown(const std::string& name, const std::string& desc,
                     const std::vector<std::string>& options, ComboCb cb = ComboCb());

    /**
     * Returns the selected value of the given dropdown
     */
    int getSelectedDropdownOption(const std::string& name) const;

    /**
     * Sets the value of the dropdown
     */
    void setDropdownSelection(const std::string& name, int i);

    /**
     * Returns the opposite direction of the fill direction
     */
    sfg::Box::Orientation getAntiFillDir();

    /**
     * Returns the fill direction
     */
    sfg::Box::Orientation getFillDir();

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
