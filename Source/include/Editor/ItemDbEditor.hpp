#ifndef ITEMDBEDITOR_HPP
#define ITEMDBEDITOR_HPP

#include "Shared/GUI/CellTable.hpp"
#include <SFGUI/ComboBox.hpp>

/**
 * GUI class to allow editing of the item db
 *
 * \ingroup Editor
 */
class ItemDbEditor {
	sfg::Desktop& desktop;
	sfg::Notebook::Ptr owner;
	sfg::Box::Ptr container;
	sfg::ComboBox::Ptr cats;

	CellTable* data;

	/**
	 * Opens a window to make a new item or edit an existing item
	 */
	void doItem(int id = 0);

	/**
	 * Saves the item db
	 */
	void save();

	/**
	 * Updates the GUI based on the items in the ItemDB
	 */
	void updateGui();

public:
	/**
	 * Constructs the GUI elements from the editor
	 */
	ItemDbEditor(sfg::Desktop& dk, sfg::Notebook::Ptr parent);

	/**
	 * Updates components of the editor
	 */
	void update();
};

#endif // ITEMDBEDITOR_HPP
