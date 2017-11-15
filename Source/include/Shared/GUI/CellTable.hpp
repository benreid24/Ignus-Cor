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
		std::vector<sfg::Widget::Ptr> columns;
		sfg::Button::Ptr delButton;
		sfg::Box::Ptr container;

		Row(int id, std::vector<std::vector<std::string> > colVals, sfg::Box::Ptr parent, CellTable* owner);
		Row() = default;
	};

	std::map<std::string,int> columns; //name -> index
	std::map<std::string,std::vector<std::string> > columnValues; //name -> list of values. Empty means Entry, otherwise ComboBox
	std::map<int,Row> rows; //uuid -> row

	sfg::Box::Ptr container, titleBox, rowBox;
	sfg::ScrolledWindow::Ptr cellArea;
	sfg::Button::Ptr appendBut;

	/**
	 * Deletes the given row
	 */
	void removeRow(int uuid);

public:
	/**
	 * Initializes GUI elements
	 */
	CellTable(sfg::Box::Ptr parent, const std::string& title);

	/**
	 * Adds a named column to the table
	 */
	void appendColumn(const std::string& name, std::vector<std::string> values = std::vector<std::string>());

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
