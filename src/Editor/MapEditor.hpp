#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP

#include <SFML/Graphics.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFGUI/SFGUI.hpp>
#include "Shared/Maps/Map.hpp"
#include "Shared/Maps/Tileset.hpp"
#include "Editor/GUI/WrappingBox.hpp"
#include "Editor/Helpers/LayerButtons.hpp"

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
	sfg::Box::Ptr container, page;
	sfg::Notebook::Ptr tabs;
	sfg::Canvas::Ptr mapArea;
	sf::RenderTexture mapAreaTarget;
	sf::Sprite mapAreaSprite;
	enum Tool {
		Set,
		Select,
		Events,
		AI,
		Spawns,
		Spawners,
        Items,
        Lights
	}curTool;

	//General area
	sfg::Label::Ptr nameLabel, widthLabel,  heightLabel, firstyLabel, firsttopLabel, musicLabel, posLabel;
    sfg::Button::Ptr newBut, loadBut, saveBut, propsBut, noneBut, allBut;
    sfg::RadioButton::Ptr aiBut, itmBut, spwnBut, spwnrBut, lightBut, evtBut, setBut, selBut;
    sfg::Button::Ptr addLayerBeforeBut, addLayerAfterBut, delLayerBut;
    LayerButtons layerButtons;

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
    sfg::Button::Ptr addAnimBut, loadAnimBut, delAnimBut;
    sfg::ToggleButton::Ptr noAnimBut;
    std::map<int, sfg::ToggleButton::Ptr> animButs;
    WrappingBox animBox;
    int selectedAnim;

    //Collisions page
    sfg::Box::Ptr collisionPage;
    std::vector<sfg::ToggleButton::Ptr> collisionButs;
    WrappingBox collisionBox;
    int selectedCollision;

    //Editor data
    sfg::Desktop& desktop;
    sfg::Notebook::Ptr owner;
    Map* mapData;
    std::string mapFolder;
    Tileset tileset;
    sf::Vector2i selCorner1, selCorner2;

    //Helper functions

    /**
     * Returns the position of the mouse on the map
     */
	sf::Vector2i getMouseTilePos();

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
	 * Opens a window to set map properties
	 */
	void editProperties();

	/**
	 * Sets the selection to be the entire map
	 */
	void selectAll();

	/**
	 * Clears the selection
	 */
	void clearSelection();

	/**
	 * Adds a layer
	 */
	void addLayer(int offset);

	/**
	 * Removes the selected layer
	 */
	void removeLayer();

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
	 * GUI function to invoke animation editor to create map anim
	 */
	void createAnim();

	/**
	 * GUI function to add animation
	 */
	void addAnim();

	/**
     * GUI function to import existing animation from global repo of anims
     */
	void loadAnim();

	/**
	 * GUI function to remove an animation
	 */
	void removeAnim();

	/**
	 * Updates the clicked tile based on the current tool
	 */
	void mapClicked();

	/**
	 * Handles MapEvent related clicking
	 */
	void mapEventHandler(sf::Vector2i clickPos);

	/**
	 * Handles Light related clicking. Note: position is in pixels
	 */
	void lightHandler(sf::Vector2i pos);

	/**
	 * Handles Spawn related clicking
	 */
	void spawnHandler(sf::Vector2i pos);

	/**
	 * Handles Spawner related clicking
	 */
	void spawnerHandler(sf::Vector2i pos);

	/**
	 * Handles Item related clicking. Note: position is in pixels
	 */
	void itemHandler(sf::Vector2i pos);

	/**
	 * Updates the info labels
	 */
	void updateInfo();

	/**
	 * Updates the currently selected tile or animation (GUI button)
	 */
	void updateSelected(const std::string& type, int id);

	/**
	 * Updates the currently active tool
	 */
	void updateTool();

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
