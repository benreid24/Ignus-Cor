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

		/*mapArea->Bind();
		mapArea->Draw(rect);
		mapArea->Display();
		mapArea->Unbind();*/

		sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();
		sleep(milliseconds(20));
	}

	return 0;
}
