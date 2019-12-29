#include "Editor/Helpers/Dialogs.hpp"
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Label.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
using namespace std;
using namespace sfg;

extern sf::RenderWindow sfWindow;
extern SFGUI sfgui;

bool yesnobox(Desktop& dk, Widget::Ptr parent,
              const string& title, const string& prompt) {
    parent->SetState(Widget::State::INSENSITIVE);

    sfg::Window::Ptr window = sfg::Window::Create();
    window->SetTitle(title);
    Box::Ptr winBox = Box::Create(Box::Orientation::VERTICAL,5);
	window->Add(winBox);
	dk.Add(window);

	Label::Ptr text = Label::Create(prompt);
	winBox->Pack(text,false,false);

	Box::Ptr butBox = Box::Create(Box::Orientation::HORIZONTAL,5);
	bool yes = false;
	bool no = false;

	Button::Ptr yesBut = Button::Create("Yes");
	yesBut->GetSignal(Button::OnLeftClick).Connect( [&yes] { yes = true; });
	butBox->Pack(yesBut,false,false);

	Button::Ptr noBut = Button::Create("No");
	noBut->GetSignal(Button::OnLeftClick).Connect( [&no] { no = true; });
	butBox->Pack(noBut,false,false);

	winBox->Pack(butBox,false,false);
	while (sfWindow.isOpen() && !yes && !no) {
		sf::Event wv;
		while (sfWindow.pollEvent(wv)) {
			dk.HandleEvent(wv);

			if (wv.type==sf::Event::Closed)
				sfWindow.close();
		}
        dk.Update(30/1000);

        dk.BringToFront(window);
        sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();
		sf::sleep(sf::milliseconds(30));
    }

    dk.Remove(window);
    parent->SetState(Widget::State::NORMAL);
    if (yes)
        return true;
    return false;
}

void showmessage(Desktop& dk, Widget::Ptr parent,
              const string& title, const string& prompt) {
    parent->SetState(Widget::State::INSENSITIVE);

    sfg::Window::Ptr window = sfg::Window::Create();
    window->SetTitle(title);
    Box::Ptr winBox = Box::Create(Box::Orientation::VERTICAL,5);
	window->Add(winBox);
	dk.Add(window);

	Label::Ptr text = Label::Create(prompt);
	winBox->Pack(text,false,false);

	bool ok = false;
	Button::Ptr okBut = Button::Create("OK");
	okBut->GetSignal(Button::OnLeftClick).Connect( [&ok] { ok = true; });
	winBox->Pack(okBut,false,false);

	while (sfWindow.isOpen() && !ok) {
		sf::Event wv;
		while (sfWindow.pollEvent(wv)) {
			dk.HandleEvent(wv);

			if (wv.type==sf::Event::Closed)
				sfWindow.close();
		}
        dk.Update(30/1000);

        dk.BringToFront(window);
        sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();
		sf::sleep(sf::milliseconds(30));
    }

    dk.Remove(window);
    parent->SetState(Widget::State::NORMAL);
}
