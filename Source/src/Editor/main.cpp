#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widget.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Notebook.hpp>
#include <SFGUI/Label.hpp>
using namespace std;
using namespace sf;

int main() {
	RenderWindow sfWindow(VideoMode(1200,800,32), "Ignis Cor Editor", Style::Titlebar|Style::Close);
	sfWindow.resetGLStates();

	sfg::SFGUI sfgui;

	sfg::Window::Ptr window = sfg::Window::Create();
	sfg::Desktop desktop;

	sfg::Notebook::Ptr tabs = sfg::Notebook::Create();
	tabs->AppendPage(window, sfg::Label::Create("Map"));
	tabs->AppendPage(window, sfg::Label::Create("Animations"));
	tabs->AppendPage(window, sfg::Label::Create("Conversation"));
	tabs->AppendPage(window, sfg::Label::Create("Items"));
	tabs->AppendPage(window, sfg::Label::Create("Weapons"));
	tabs->AppendPage(window, sfg::Label::Create("Spells"));
	tabs->AppendPage(window, sfg::Label::Create("Creatures"));
	tabs->AppendPage(window, sfg::Label::Create("Scripts"));
	tabs->SetAllocation(FloatRect(0,0,1200,40));

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

		sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();
		sleep(milliseconds(20));
	}

	return 0;
}
