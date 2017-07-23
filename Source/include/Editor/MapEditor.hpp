#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP

#include <SFML/Graphics.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFGUI/SFGUI.hpp>
#include "Shared/Maps/Map.hpp"
#include "Shared/Maps/Tileset.hpp"
#include "Shared/GUI/WrappingBox.hpp"

extern sfg::SFGUI sfgui;
extern sf::RenderWindow sfWindow;

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
	sf::RenderTexture mapAreaTarget;
	sf::Sprite mapAreaSprite;

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
    sfg::Desktop& desktop;
    sfg::Notebook::Ptr owner;
    Map* mapData;
    SoundEngine soundEngine;
    EntityManager* entityManager;
    std::string mapFolder;
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
	 * GUI function to remove a tile
	 */
	void removeTile();

	/**
	 * GUI function to add animation
	 */
	void addAnim();

	/**
	 * GUI function to remove an animation
	 */
	void removeAnim();

	/**
	 * Updates the clicked tile based on the current tool
	 */
	void mapClicked();

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

	/**
	 * Updates the editor. Call this once every update frame
	 */
	void update();

	/**
	 * Renders the SFML components of the map. Call this after drawing the GUI
	 */
	void render();
};

#endif // MAPEDITOR_HPP
