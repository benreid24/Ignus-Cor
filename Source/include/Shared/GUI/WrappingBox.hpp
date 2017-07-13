#ifndef WRAPPINGBOX_HPP
#define WRAPPINGBOX_HPP

#include <SFGUI/Widgets.hpp>
#include <vector>
#include <memory>

/**
 * Helper SFGUI container class that can dynamically create a grid of Widgets
 * It does this with a Box containing Boxes to emulate rows/columns
 */
class WrappingBox {
	sfg::Box::Orientation cutDir, rowDir;
	double space;
	sfg::Box::Ptr container;
	std::vector<sfg::Box::Ptr> cuts;
	std::vector<sfg::Widget::Ptr> members;
	int curCutSize, maxCutSize;

    /**
     * Deletes all of the widgets in the boxes and reassigns them
     */
	void refresh();

	/**
	 * Clears out the GUI elements cleanly
	 */
    void clearGui();

    /**
     * Adds the passed widget to the gui
     */
	void addWidgetToGui(sfg::Widget::Ptr widget);

public:
	/**
	 * Creates a WrappingBox object
	 *
	 * \param cutDir What direction to initially add items before wrapping
	 * \param spacing The spacing between items and cuts
	 * \param cutSize How many elements to add to each row/column before wrapping
	 * \return A pointer to the created WrappingBox
	 */
	WrappingBox(sfg::Box::Orientation cutDir, double spacing, int cutSize);

	/**
	 * Adds the internal elements to the specified ScrolledWindow
	 */
	void setParent(sfg::ScrolledWindow::Ptr parent);

	/**
	 * Adds the internal elements to the specified Box
	 */
	void setParent(sfg::Box::Ptr parent);

	/**
	 * Adds the given widget to the box
	 */
	void addWidget(sfg::Widget::Ptr widget);

	/**
	 * Removes the given widget from the box
	 */
	void removeWidget(sfg::Widget::Ptr widget);

	/**
	 * Removes all widgets from the box
	 */
	void clear();
};

#endif // WRAPPPINGBOX_HPP
