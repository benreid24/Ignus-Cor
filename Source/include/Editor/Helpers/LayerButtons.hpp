#ifndef LAYERBUTTONS_HPP
#define LAYERBUTTONS_HPP

#include <SFGUI/Widgets.hpp>
#include <vector>

/**
 * Helper class to manage the layer buttons in the MapEditor
 *
 * \ingroup Editor
 */
class LayerButtons {
	struct Layer {
		sfg::Box::Ptr wrapper;
		sfg::Label::Ptr label;
		sfg::RadioButton::Ptr activeBut;
		sfg::ToggleButton::Ptr visibleBut;
	};

	sfg::Box::Ptr container;
	std::vector<Layer> layerButs;

public:
	/**
	 * Creates internal structures
	 */
	LayerButtons();

	/**
	 * Sets the number of layers
	 */
	void setLayers(int nLayers);

	/**
	 * Adds a layer
	 */
	void addLayer();

	/**
	 * Removes a layer
	 */
	void removeLayer();

	/**
	 * Clears all layers
	 */
	void clear();

	/**
	 * Returns the currently selected layer
	 */
	int getCurrentLayer();

	/**
	 * Returns a list of visible layers
	 */
	std::vector<int> getVisibleLayers();

	/**
	 * Adds the GUI elements to the given parent
	 */
	void addToParent(sfg::Box::Ptr parent);
};

#endif // LAYERBUTTONS_HPP
