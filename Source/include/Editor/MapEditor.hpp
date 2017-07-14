#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP

#include <SFML/Graphics.hpp>
#include <SFGUI/Widgets.hpp>
#include "Shared/Maps/Map.hpp"
#include "Shared/Maps/Tileset.hpp"
#include "Shared/GUI/WrappingBox.hpp"

/*
sfml_canvas->Bind();
sfml_canvas->Clear( sf::Color( 0, 0, 0, 0 ) );

// Draw the SFML Sprite.
sfml_canvas->Draw( sprite );

sfml_canvas->Display();
sfml_canvas->Unbind();

*/

/**
 * \defgroup Editor Classes and functions contained within the main editor program
 */

/**
 * GUI class that owns all the sfg elements for the map tab and handles the updating of everything
 *
 * \ingroup Editor
 */
class MapEditor {
	//Editor
	sfg::Box::Ptr container;
	sfg::Notebook::Ptr tabs;
	sfg::Canvas::Ptr mapArea;

	//General page
	sfg::Box::Ptr generalPage;
    sfg::Button::Ptr newBut, loadBut, saveBut, propsBut, noneBut, allBut;
    sfg::RadioButton::Ptr aiBut, itmBut, spwnBut, lightBut, evtBut, colsBut, setBut, selBut;
    sfg::Button::Ptr addLayerBut, delLayerBut;
    sfg::Box::Ptr layerBox;
    std::vector<sfg::Button::Ptr> layerButs;

    //Tiles page
    sfg::ScrolledWindow::Ptr tilesPageScroll;
    sfg::Box::Ptr tilesPage;
    sfg::Button::Ptr addTileBut, delTileBut, addTileFolderBut;
    sfg::ToggleButton::Ptr noTileBut;
    std::map<int, sfg::ToggleButton::Ptr> tileButs;
    WrappingBox tileBox;
    int selectedTile;

    //Animations page
    sfg::ScrolledWindow::Ptr animPageScroll;
    sfg::Box::Ptr animPage;
    sfg::Button::Ptr addAnimBut, delAnimBut;
    sfg::ToggleButton::Ptr noAnimBut;
    std::map<int, sfg::ToggleButton::Ptr> animButs;
    WrappingBox animBox;
    int selectedAnim;

    //Editor data
    Map* mapData;
    Tileset tileset;
    sf::Vector2i selCorner1, selCorner2;

    //Helper functions

    /**
     * Applies the current operation to the selection
     */
	void applyToSelection();

	/**
	 * Updates GUI based on tileset
	 */
	void syncGuiWithTileset();

	/**
	 * Creates a new Map, saving the old one if it exists
	 */
	void newMap();

	/**
	 * Loads an existing map
	 */
	void loadMap();

	/**
	 * Saves the current map
	 */
	void save();

	/**
	 * GUI function to add tile
	 */
	void addTile();

	/**
	 * GUI function to add tiles in a folder
	 */
	void addTileFolder();

	/**
	 * GUI function to add animation
	 */
	void addAnim();

	/**
	 * Updates the currently selected tile or animation
	 */
	void updateSelected(const std::string& type, int id);

public:
	/**
	 * Initializes all of the menu elements
	 *
	 * \param desktop A reference to the desktop containing everything
	 * \param parent A pointer to the notebook to add the GUI to
	 */
	MapEditor(sfg::Desktop& desktop, sfg::Notebook::Ptr parent);
};

#endif // MAPEDITOR_HPP
