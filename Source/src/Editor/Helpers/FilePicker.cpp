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

FilePicker::FilePicker(Desktop& o, Widget::Ptr p, string dname, string dir, string ext, bool embed)
: owner(o), parent(p), searchDir(dir), extension(ext) {
	FilePicker* me = this;

	dispName = dname;
	state = embed ? Embedded : Picking;

	if (!embed) {
        window = Window::Create();
        window->SetTitle("Pick "+dispName);
        window->SetRequisition(sf::Vector2f(450,550));
	}

	container = Box::Create(Box::Orientation::HORIZONTAL,5);
	curFile = Label::Create("Selected "+dispName+":");
	pickButton = Button::Create("Select");
	pickButton->GetSignal(Button::OnLeftClick).Connect( [me] { me->setState(Chosen); });
	cancelButton = Button::Create("Cancel");
	cancelButton->GetSignal(Button::OnLeftClick).Connect( [me] { me->setState(Canceled); });

	whole = Box::Create(Box::Orientation::VERTICAL,7);
	whole->Pack(curFile,false,false);
	whole->Pack(container,false,false);

	folder = Directory::get(dir, ext);
	root = folder;
	refreshFiles();
	needsUpdate = false;

	if (embed) {
        Box* cont = static_cast<Box*>(parent.get());
        cont->Pack(whole, true, true);
	}
    else {
        Box::Ptr t = Box::Create(Box::Orientation::HORIZONTAL,5);
        whole->Pack(t,false,false);
        t->Pack(pickButton);
        t->Pack(cancelButton);
        window->Add(whole);
        owner.Add(window);
    }
}

void FilePicker::refreshFiles() {
    FilePicker* me = this;

	//Populate folders
	ScrolledWindow::Ptr subDirButtons = ScrolledWindow::Create();
	subDirButtons->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
	subDirButtons->SetRequisition(sf::Vector2f(200,500));
	Box::Ptr dirBox = Box::Create(Box::Orientation::VERTICAL,2);
	subDirButtons->AddWithViewport(dirBox);
	vector<Directory::Ptr> dirs = folder->getSubDirectories();

	if (folder->hasParent()) {
        string nm = "~Up~";
		Button::Ptr dirBut = Button::Create(nm);
		dirBut->GetSignal(Button::OnLeftClick).Connect( [me,nm] { me->setDirectory(nm); });
		dirBox->Pack(dirBut,false,false);
	}
	for (unsigned int i = 0; i<dirs.size(); ++i) {
        string nm = dirs[i]->getName();
		Button::Ptr dirBut = Button::Create(nm);
		dirBut->GetSignal(Button::OnLeftClick).Connect( [me,nm] { me->setDirectory(nm); });
		dirBox->Pack(dirBut,false,false);
	}

	//Populate files
	RadioButtonGroup::Ptr butGroup(nullptr);
	vector<string> files = folder->getFiles();
	Box::Ptr tBox = Box::Create(Box::Orientation::VERTICAL,2);
    ScrolledWindow::Ptr fileBox = ScrolledWindow::Create();
    fileBox->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    fileBox->AddWithViewport(tBox);
    fileBox->SetRequisition(sf::Vector2f(200,500));
	for (unsigned int i = 0; i<files.size(); ++i) {
        string f = files[i];
        RadioButton::Ptr but = RadioButton::Create(f);
        if (butGroup.get()==nullptr)
            butGroup = but->GetGroup();
        else
            but->SetGroup(butGroup);
        but->GetSignal(Button::OnLeftClick).Connect( [me,f] { me->setChoice(f); });
        tBox->Pack(but,false,false);
	}

	//Pack windows
	container->Pack(subDirButtons);
	container->Pack(Separator::Create(Separator::Orientation::VERTICAL),false,false);
	container->Pack(fileBox);
}

void FilePicker::setChoice(string file) {
	chosenFile = file;
	curFile->SetText("Selected "+dispName+": "+file);
}

void FilePicker::setDirectory(string subDir) {
	if (subDir == "~Up~") {
        if (folder->hasParent())
            folder = folder->getParent();
	}
	else {
        vector<Directory::Ptr> dirs = folder->getSubDirectories();
        for (unsigned int i = 0; i<dirs.size(); ++i) {
            if (dirs[i]->getName() == subDir) {
                folder = dirs[i];
                break;
            }
        }
	}
	needsUpdate = true;
}

void FilePicker::setState(State s) {
	if (s==Chosen && chosenFile.size()>0)
		state = Chosen;
	else if (s==Canceled && state!=Embedded)
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

void FilePicker::update() {
    if (needsUpdate) {
        needsUpdate = false;
        container->RemoveAll();
        refreshFiles();
    }
}
