#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include "Editor/MapEditor.hpp"
#include "Editor/ItemDbEditor.hpp"
#include "Editor/AnimationEditor.hpp"
#include "Editor/ConversationEditor.hpp"
#include "Editor/ScriptEditor.hpp"
#include "Shared/Properties.hpp"
using namespace std;
using namespace sf;

sfg::SFGUI sfgui;
RenderWindow sfWindow(VideoMode(1600,900,32), "Ignis Cor Editor", Style::Titlebar|Style::Close);

int main() {
	sfWindow.resetGLStates();
	Properties::PrimaryMenuFont.loadFromFile(Properties::FontPath+"Abea.ttf");

	sfg::Desktop desktop;

	sfg::Notebook::Ptr tabs = sfg::Notebook::Create();
	MapEditor mapEditor(desktop, tabs);
	ItemDbEditor itemEditor(desktop,tabs);
	AnimationEditor animEditor(desktop,tabs);
	ConversationEditor convEditor(desktop, tabs);
	ScriptEditor scriptEditor(desktop, tabs);

	tabs->AppendPage(sfg::Label::Create("make this"),sfg::Label::Create("NPC Editor"));
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
		else if (tabs->GetCurrentPage()==1 && sfWindow.hasFocus())
			itemEditor.update();
        else if (tabs->GetCurrentPage()==2 && sfWindow.hasFocus())
            animEditor.update();
        else if (tabs->GetCurrentPage()==3 && sfWindow.hasFocus())
            convEditor.update();
        else if (tabs->GetCurrentPage()==4 && sfWindow.hasFocus())
            scriptEditor.update();

		sfWindow.clear(Color(50,50,50));
		sfgui.Display(sfWindow);
		sfWindow.display();
		sleep(milliseconds(20));
	}

	return 0;
}
