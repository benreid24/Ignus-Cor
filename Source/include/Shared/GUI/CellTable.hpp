#ifndef CELLTABLE_HPP
#define CELLTABLE_HPP

#include <SFGUI/Widgets.hpp>
#include <map>
#include <vector>
#include <string>

/**
 * Helper class to create a table of editable cells
 */
class CellTable {
	/**
	 * Wrapper struct for row data
     */
	struct Row {
		std::vector<sfg::Entry::Ptr> columns;
		sfg::Button::Ptr delButton;

		Row(int nCols, sfg::Box::Ptr parent, CellTable* owner);
	};

	std::map<std::string,int> columns; //name -> index
	std::map<int,Row> rows; //uuid -> row
	bool dirty; //if data changed since last check

	sfg::Box::Ptr container, titleBox;
	sfg::ScrolledWindow::Ptr cellArea;
	sfg::Button::Ptr appendBut;

	/**
	 * Deletes the given row
	 */
	void removeRow(int uuid);

	/**
	 * Called when data is changed
	 */
	void dataChanged();

public:
	/**
	 * Initializes GUI elements
	 */
	CellTable(sfg::Box::Ptr parent);

	/**
	 * Adds a named column to the table
	 */
	void appendColumn(const std::string& name);

	/**
	 * Returns the amount of rows in the table
	 */
	int rowCount();

	/**
	 * Returns the value of the given cell as a string
	 */
	std::string getCellValue(int index, const std::string& colName);

	/**
	 * Returns the value of the given cell as an int
	 */
	int getCellValueAsInt(int index, const std::string& colName);

	/**
	 * Returns true if data has been modified since this was last called
	 */
	bool isDirty();

	/**
	 * Adds a new row
	 */
	void appendRow();

	/**
	 * Sets the given cell to the given string
	 */
	void setCellValue(int index, const std::string& colName, const std::string& value);

	/**
	 * Sets the given cell to the given int
	 */
	void setCellValue(int index, const std::string& colName, int value);
};

#endif // CELLTABLE_HPP
