#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include "Editor/MapEditor.hpp"
using namespace std;
using namespace sf;

sfg::SFGUI sfgui;
RenderWindow sfWindow(VideoMode(1600,900,32), "Ignis Cor Editor", Style::Titlebar|Style::Close);

int main() {
	sfWindow.resetGLStates();

	sfg::Desktop desktop;

	sfg::Notebook::Ptr tabs = sfg::Notebook::Create();
	MapEditor mapEditor(desktop, tabs);

	tabs->AppendPage(sfg::Label::Create("make this"),sfg::Label::Create("Animation Editor"));
	tabs->AppendPage(sfg::Label::Create("make this"),sfg::Label::Create("NPC Editor"));
	tabs->AppendPage(sfg::Label::Create("make this"),sfg::Label::Create("Conversation Editor"));
	tabs->AppendPage(sfg::Label::Create("make this"),sfg::Label::Create("Script Editor"));

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
		mapEditor.update();
		timer.restart();

		sfWindow.clear(Color(30,30,30));
		sfgui.Display(sfWindow);
		mapEditor.render();
		sfWindow.display();
		sleep(milliseconds(20));
	}

	return 0;
}
