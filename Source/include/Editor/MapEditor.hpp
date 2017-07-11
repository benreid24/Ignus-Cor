#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP

#include <SFML/Graphics.hpp>
#include <SFGUI/Widgets.hpp>
#include "Shared/Maps/Map.hpp"
#include "Shared/Maps/Tileset.hpp"

/*
box->Pack( sfg::Separator::Create() );

auto radio_button1 = sfg::RadioButton::Create( "Either this" );
auto radio_button2 = sfg::RadioButton::Create( "Or this", radio_button1->GetGroup() );
auto radio_button3 = sfg::RadioButton::Create( "Or maybe even this", radio_button1->GetGroup() );

scrolledwindow->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
scrolledwindow->AddWithViewport( scrolled_window_box );

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


	//General page
	sfg::Box::Ptr generalPage;
    sfg::Button::Ptr newBut, loadBut, saveBut, propsBut;
    sfg::RadioButton::Ptr aiBut, itmBut, spwnBut, lightBut, evtBut, colsBut, setBut, selBut, allBut, noneBut;
    sfg::Button::Ptr addLayerBut, delLayerBut;

    //Tiles page
    sfg::ScrolledWindow::Ptr tilesPageScroll;
    sfg::Box::Ptr tilesPage;
    sfg::Button::Ptr addTileBut, delTileBut, noTileBut;
    std::vector<sfg::Button::Ptr> tileButs;

    //Animations page
    sfg::ScrolledWindow::Ptr animPageScroll;
    sfg::Box::Ptr animPage;
    sfg::Button::Ptr addAnimBut, delAnimBut, noAnimBut;
    std::vector<sfg::Button::Ptr> animButs;

    //Editor data
    Map* mapData;
    Tileset* tileset;
    sf::Vector2i selCorner1, selCorner2;

    //Helper functions
    /**
     * Applies the current operation to the selection
     */
	void applyToSelection();

public:
	/**
	 * Initializes all of the menu elements
	 *
	 * \param desktop A reference to the desktop containing everything
	 * \param parent A pointer to the parent widget of the GUI
	 */
	MapEditor(sfg::Desktop& desktop, sfg::Widget::Ptr parent);
};

#endif // MAPEDITOR_HPP
