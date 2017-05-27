#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Notebook.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>
#include "Shared/Maps/Map.hpp"
#include "Shared/Maps/Tileset.hpp"

/**
 * GUI class that owns all the sfg elements for the map tab and handles the updating of everything
 */
class MapEditor {
	//stuff

public:
	MapEditor(Map& mp, Tileset& tlst); //maybe more
};

#endif // MAPEDITOR_HPP
