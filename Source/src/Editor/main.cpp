#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include "Editor/MapEditor.hpp"
#include "Editor/ItemDbEditor.hpp"
#include "Editor/version.h"
using namespace std;
using namespace sf;

sfg::SFGUI sfgui;
RenderWindow sfWindow(VideoMode(1600,900,32), string("Ignis Cor Editor ")+AutoVersion::FULLVERSION_STRING, Style::Titlebar|Style::Close);

int main() {
	sfWindow.resetGLStates();

	sfg::Desktop desktop;

	sfg::Notebook::Ptr tabs = sfg::Notebook::Create();
	MapEditor mapEditor(desktop, tabs);
	ItemDbEditor itemEditor(desktop,tabs);

	tabs->AppendPage(sfg::Label::Create("make this"),sfg::Label::Create("Animation Editor"));
	tabs->AppendPage(sfg::Label::Create("make this"),sfg::Label::Create("NPC Editor"));
	tabs->AppendPage(sfg::Label::Create("make this"),sfg::Label::Create("Conversation Editor"));

	desktop.Add(tabs);

	Clock timer;
	while (sfWindow.isOpen()) {
		Event evt;
		while (sfWindow.pollEvent(evt)) {
			desktop.HandleEvent(evt);

			if (evt.type==Event::Closed)
				sfWindow.close();
		}

		desktop.Update(timer.getElapsedTime().asSeconds());
		timer.restart();

		if (tabs->GetCurrentPage()==0 && sfWindow.hasFocus()) {
			mapEditor.update();
			mapEditor.render();
		}

		sfWindow.clear(Color(50,50,50));
		sfgui.Display(sfWindow);
		sfWindow.display();
		sleep(milliseconds(20));
	}

	return 0;
}
