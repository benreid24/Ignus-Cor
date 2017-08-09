#include "Editor/Helpers/FilePicker.hpp"
#include "Shared/Util/File.hpp"
#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/RadioButtonGroup.hpp>
#include <algorithm>
using namespace std;
using namespace sfg;

extern sfg::SFGUI sfgui;
extern sf::RenderWindow sfWindow;

FilePicker::FilePicker(Desktop& o, Widget::Ptr p, string dname, string searchDir, string extension) : owner(o), parent(p) {
	FilePicker* me = this;

	dispName = dname;
	state = Picking;
	vector<string> rawFiles = listDirectory(searchDir,extension,true);
	for (unsigned int i = 0; i<rawFiles.size(); ++i) {
		string dir = File::getPath(rawFiles[i]);
		dir.erase(0,searchDir.size());
		if (dir[0]=='/')
			dir.erase(dir.begin());
		if (dir[dir.size()-1]=='/')
			dir.erase(dir.begin()+dir.size()-1);
		if (dir.size()==0)
			dir = "Base Directory";

		if (files.find(dir)==files.end())
			files[dir] = vector<string>();
		files[dir].push_back(File::getBaseName(rawFiles[i]));
	}

	window = Window::Create();
	window->SetTitle("Pick "+dispName);
	window->SetRequisition(sf::Vector2f(450,550));
	container = Box::Create(Box::Orientation::HORIZONTAL,5);
	curFile = Label::Create("Selected "+dispName+":");
	pickButton = Button::Create("Select");
	pickButton->GetSignal(Button::OnLeftClick).Connect( [me] { me->setState(Chosen); });
	cancelButton = Button::Create("Cancel");
	cancelButton->GetSignal(Button::OnLeftClick).Connect( [me] { me->setState(Canceled); });
	subDirButtons = ScrolledWindow::Create();
	subDirButtons->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
	subDirButtons->SetRequisition(sf::Vector2f(200,450));
	Box::Ptr dirBox = Box::Create(Box::Orientation::VERTICAL,2);
	subDirButtons->AddWithViewport(dirBox);

	RadioButtonGroup::Ptr butGroup(nullptr);
	for (map<string,vector<string> >::iterator i = files.begin(); i!=files.end(); ++i) {
		string d = i->first;
		Button::Ptr dirBut = Button::Create(d);
		dirBut->GetSignal(Button::OnLeftClick).Connect( [me,d] { me->setDirectory(d); });
		dirBox->Pack(dirBut,false,false);

		Box::Ptr tBox = Box::Create(Box::Orientation::VERTICAL,2);
		fileButtons[i->first] = ScrolledWindow::Create();
		fileButtons[i->first]->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
		fileButtons[i->first]->AddWithViewport(tBox);
		fileButtons[i->first]->SetRequisition(sf::Vector2f(200,450));
		for (unsigned int j = 0; j<i->second.size(); ++j) {
			RadioButton::Ptr but = RadioButton::Create(File::getBaseName(i->second[j]));
			if (butGroup.get()==nullptr)
                butGroup = but->GetGroup();
			else
				but->SetGroup(butGroup);
			string f = i->second[j];
			but->GetSignal(Button::OnLeftClick).Connect( [me,f] { me->setChoice(f); });
			tBox->Pack(but,false,false);
		}
	}

	container->Pack(subDirButtons);
	container->Pack(Separator::Create(Separator::Orientation::VERTICAL),false,false);
	if (fileButtons.find("Base Directory")!=fileButtons.end())
		container->Pack(fileButtons["Base Directory"]);
	else if (fileButtons.size()>0)
		container->Pack(fileButtons.begin()->second);

	Box::Ptr whole = Box::Create(Box::Orientation::VERTICAL,7);
	Box::Ptr t = Box::Create(Box::Orientation::HORIZONTAL,5);
	t->Pack(pickButton);
	t->Pack(cancelButton);

	whole->Pack(curFile,false,false);
	whole->Pack(container,false,false);
	whole->Pack(t,false,false);
	window->Add(whole);
	owner.Add(window);
}

void FilePicker::setChoice(string file) {
	chosenFile = file;
	curFile->SetText("Selected "+dispName+": "+file);
}

void FilePicker::setDirectory(string subDir) {
	container->RemoveAll();
	container->Pack(subDirButtons);
	container->Pack(Separator::Create(Separator::Orientation::VERTICAL),false,false);
	container->Pack(fileButtons[subDir]);
}

void FilePicker::setState(State s) {
	if (s==Chosen && chosenFile.size()>0)
		state = Chosen;
	else if (s==Canceled)
		state = Canceled;
}

string FilePicker::getChoice() {
	return chosenFile;
}

bool FilePicker::pickFile() {
	parent->SetState(Widget::State::INSENSITIVE);

	while (sfWindow.isOpen() && state==Picking) {
		sf::Event evt;
		while (sfWindow.pollEvent(evt)) {
			owner.HandleEvent(evt);

			if (evt.type==sf::Event::Closed)
				sfWindow.close();
		}
		owner.BringToFront(window);
        owner.Update(30/1000);

        sfWindow.clear();
		sfgui.Display(sfWindow);
		sfWindow.display();

		sf::sleep(sf::milliseconds(30));
	}
	parent->SetState(Widget::State::NORMAL);
    owner.Remove(window);
    return state==Chosen;
}
