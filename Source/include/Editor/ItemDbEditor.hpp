#ifndef ITEMDBEDITOR_HPP
#define ITEMDBEDITOR_HPP

#include "Shared/GUI/CellTable.hpp"

/**
 * GUI class to allow editing of the item db
 *
 * \ingroup Editor
 */
class ItemDbEditor {
	sfg::Desktop& desktop;
	sfg::Notebook::Ptr owner;
	sfg::Box::Ptr container;

	CellTable* data;

public:
	ItemDbEditor(sfg::Desktop& dk, sfg::Notebook::Ptr parent);
};

#endif // ITEMDBEDITOR_HPP
