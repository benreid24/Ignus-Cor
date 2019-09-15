#ifndef CELLTABLE_HPP
#define CELLTABLE_HPP

#include <SFGUI/Widgets.hpp>
#include <map>
#include <vector>
#include <string>

/**
 * Helper class to create a table of cells that can be added to, edited (separately), and deleted
 */
class CellTable {
    std::vector<std::string> columns;
	std::map<int,std::vector<sfg::Widget::Ptr> > rows; //uuid -> list of widgets for row
	int editId; //id of cell to edit
	int toRemove; //id of cell to delete

	sfg::Box::Ptr parent;
	sfg::ScrolledWindow::Ptr cellArea;
	sfg::Table::Ptr table;

	/**
	 * Marks the given row to be edited
	 */
	void editRow(int uuid);

	/**
	 * Performs deletion of the row with the given uuid
	 */
    void doRowDelete(int uuid);

public:
	/**
	 * Initializes GUI elements
	 */
	CellTable(sfg::Box::Ptr parent, const std::vector<std::string>& colNames);

	/**
	 * Returns the set of ids in the table
	 */
	std::vector<int> getIds();

	/**
	 * Adds a new row with the given columns and id. Adds an edit and delete button to the end
	 */
	void appendRow(int id, const std::vector<std::string>& cols, bool addButs = true);

	/**
	 * Deletes the given row
	 */
	void removeRow(int uuid, bool immediate = true);

	/**
	 * Deletes all rows
	 */
	void removeAll();

	/**
	 * Returns the id of the last row to have "Edit" clicked, -1 if none
	 */
	int getEditCell();

	/**
	 * Returns the id of the last row to have "Delete" clicked, -1 if none
	 */
    int getDeleteCell();

	/**
	 * Sorts the rows based on id
	 */
	void reorder();
};

#endif // CELLTABLE_HPP
