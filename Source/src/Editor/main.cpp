#include <iostream>
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
using namespace std;
using namespace sf;

int main() {
	RenderWindow sfWindow(VideoMode(1920,1080,32), "Ignis Cor Editor", Style::Titlebar|Style::Close);
	sfWindow.resetGLStates();
	RectangleShape rect(Vector2f(200,200));
	rect.setFillColor(Color::Red);

	sfg::SFGUI sfgui;

	sfg::Window::Ptr window = sfg::Window::Create();
	sfg::Desktop desktop;

	sfg::Box::Ptr mapGeneral = sfg::Box::Create();
	mapGeneral->Pack(sfg::Button::Create("Load"));
	mapGeneral->Pack(sfg::Button::Create("Save"));

	sfg::Notebook::Ptr mapTabs = sfg::Notebook::Create();
	mapTabs->AppendPage(mapGeneral,sfg::Label::Create("General"));
	mapTabs->AppendPage(sfg::Label::Create(),sfg::Label::Create("Tiles"));
	mapTabs->AppendPage(sfg::Label::Create(),sfg::Label::Create("Animations"));
	mapTabs->SetRequisition(Vector2f(500,1040));

	sfg::Canvas::Ptr mapArea = sfg::Canvas::Create();
	mapArea->SetRequisition(Vector2f(1480,1040));

	sfg::Box::Ptr box = sfg::Box::Create();
	box->Pack(mapTabs);
	box->Pack(mapArea);

	sfg::Notebook::Ptr tabs = sfg::Notebook::Create();
	tabs->AppendPage(box, sfg::Label::Create("Map"));
	tabs->AppendPage(window, sfg::Label::Create("Animations"));
	tabs->AppendPage(window, sfg::Label::Create("Conversation"));
	tabs->AppendPage(window, sfg::Label::Create("Items"));
	tabs->AppendPage(window, sfg::Label::Create("Weapons"));
	tabs->AppendPage(window, sfg::Label::Create("Spells"));
	tabs->AppendPage(window, sfg::Label::Create("Creatures"));
	tabs->AppendPage(window, sfg::Label::Create("Scripts"));
	tabs->SetAllocation(FloatRect(0,0,1920,1080));

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

		mapArea->Bind();
		mapArea->Draw(rect);
		mapArea->Display();
		mapArea->Unbind();

		sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();
		sleep(milliseconds(20));
	}

	return 0;
}
