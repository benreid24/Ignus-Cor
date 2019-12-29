#ifndef EDITABLECELLTABLE_HPP
#define EDITABLECELLTABLE_HPP

#include <SFGUI/Widgets.hpp>
#include <vector>
#include <string>

/**
 * Built on top of CellTable. Allows editing
 *
 * \ingroup Editor
 */
class EditableCellTable {
public:
    /**
	 * Initializes GUI elements
	 */
	EditableCellTable(sfg::Desktop& dk, sfg::Widget::Ptr parent,
        const std::vector<std::string>& colNames, const std::string& typeName = "Row", int width = 0);

	/**
	 * Clears all rows
	 */
    void clear();

    /**
     * Appends a row with the given column values, or empty
     */
    void appendRow(const std::vector<std::string>& values);

    /**
     * Returns all row values
     */
    std::vector<std::vector<std::string> > getAllValues();

    /**
     * Show or hide the table
     */
    void show(bool show);

    /**
     * Updates the table
     */
    void update();

    /**
     * Adds the container to the given parent
     */
    void addToParent(sfg::Box::Ptr parent);

private:
    sfg::Desktop& desktop;
    std::vector<sfg::Label::Ptr> headerRow;
    sfg::Widget::Ptr parent;
    sfg::Box::Ptr container;
    sfg::ScrolledWindow::Ptr box;
    sfg::Table::Ptr table;
    sfg::Button::Ptr appendBut;
    bool appendPressed;
    int delUuid;

    struct Row {
        int uuid;
        std::vector<sfg::Entry::Ptr> entries;
        sfg::Button::Ptr delBut;

        typedef std::shared_ptr<Row> Ptr;
        static Ptr create() { return Ptr(new Row()); }

    private:
        Row() = default;
        Row(const Row&) = delete;
        Row& operator=(const Row&) = delete;
    };
    std::vector<Row::Ptr> rows;
    std::string typeName;
    int nextUuid;

    /**
     * Refreshes all GUI elements
     */
    void refreshGui();
};

#endif // EDITABLECELLTABLE_HPP
