#ifndef CELLTABLE_HPP
#define CELLTABLE_HPP

#include <SFGUI/Widgets.hpp>
#include <map>
#include <vector>
#include <string>

/**
 * Helper function to pack a list of strings into a row with separators
 */
sfg::Box::Ptr packRow(std::vector<std::string> cells);

/**
 * Helper class to create a table of cells that can be added to, edited (separately), and deleted
 */
class CellTable {
	std::map<int,sfg::Box::Ptr> rows; //uuid -> row
	int editId; //id of cell to edit

	sfg::Box::Ptr parent;
	sfg::ScrolledWindow::Ptr cellArea;
	sfg::Box::Ptr cellBox;

	/**
	 * Marks the given row to be edited
	 */
	void editRow(int uuid);

	/**
	 * Sorts the rows based on id
	 */
	void reorder();

public:
	/**
	 * Initializes GUI elements
	 */
	CellTable(sfg::Box::Ptr parent);

	/**
	 * Returns the set of ids in the table
	 */
	std::vector<int> getIds();

	/**
	 * Adds a new row with the given Box. Adds an edit and delete button to the end
	 */
	void appendRow(int id, sfg::Box::Ptr row);

	/**
	 * Deletes the given row
	 */
	void removeRow(int uuid);

	/**
	 * Returns the id of the last cell to have "Edit" clicked, -1 if none
	 */
	int getEditCell();
};

#endif // CELLTABLE_HPP
