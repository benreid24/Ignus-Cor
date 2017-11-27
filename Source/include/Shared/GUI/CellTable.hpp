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

	std::vector<int> toRemove;
	std::vector<int> removed;

	/**
	 * Marks the given row to be edited
	 */
	void editRow(int uuid);

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
	 * Returns the set of ids deleted since this was last checked
	 */
	std::vector<int> getDeletedIds();

	/**
	 * Adds a new row with the given Box. Adds an edit and delete button to the end
	 */
	void appendRow(int id, sfg::Box::Ptr row);

	/**
	 * Deletes the given row
	 */
	void removeRow(int uuid, bool immediate = true);

	/**
	 * Deletes all rows
	 */
	void removeAll();

	/**
	 * Returns the id of the last cell to have "Edit" clicked, -1 if none
	 */
	int getEditCell();

	/**
	 * Returns true if reorder needs to be called. Check once per frame
	 */
	bool needsReorder();

	/**
	 * Sorts the rows based on id
	 */
	void reorder();
};

#endif // CELLTABLE_HPP
