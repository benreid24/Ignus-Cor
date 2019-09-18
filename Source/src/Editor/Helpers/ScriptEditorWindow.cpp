#include "Editor/Helpers/ScriptEditorWindow.hpp"
#include "Editor/Helpers/Dialogs.hpp"
#include "Editor/Helpers/FilePicker.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Util/File.hpp"
#include <SFGUI/SFGUI.hpp>
#include <fstream>
#include <sstream>
using namespace std;
using namespace sfg;

extern sf::RenderWindow sfWindow;
extern SFGUI sfgui;

namespace {
const string NOERROR = "No errors";
}

ScriptEditorWindow::ScriptEditorWindow(Desktop& dk, Widget::Ptr pr, const string& scr, bool choose)
: desktop(dk), parent(pr), savePressed(false), openPressed(false)
, validatePressed(false), selectPressed(false) {
    window = Window::Create();
    scriptEntry = MultiLineEntry::Create(scr);
    fileEntry = Entry::Create();

    if (File::getExtension(scr) == Properties::ScriptExtension) {
        ifstream input(string(Properties::ScriptPath+scr).c_str());
        stringstream stream;
        stream << input.rdbuf();
        scriptEntry->SetText(stream.str());
        loadedFile = scr;
        fileEntry->SetText(scr);
    }

    window->SetTitle("Script Picker/Editor");
    Box::Ptr vertBox = Box::Create(Box::Orientation::VERTICAL, 5);
    Box::Ptr horBox = Box::Create(Box::Orientation::HORIZONTAL, 3);

    Button::Ptr button = Button::Create("Open New Script");
    button->GetSignal(Button::OnLeftClick).Connect( [this] { this->openPressed = true; });
    vertBox->Pack(button, false, false);
    vertBox->Pack(Separator::Create());

    horBox->Pack(Label::Create("File: "), false, false);
    horBox->Pack(fileEntry);
    button = Button::Create("Save");
    button->GetSignal(Button::OnLeftClick).Connect( [this] { this->savePressed = true; });
    horBox->Pack(button, false, false);
    vertBox->Pack(horBox, false, false);

    vertBox->Pack(Separator::Create());
    button = Button::Create("Validate");
    button->GetSignal(Button::OnLeftClick).Connect( [this] { this->validatePressed = true; });
    vertBox->Pack(button, false, false);

    scriptEntry->SetRequisition(sf::Vector2f(800, 600));
    vertBox->Pack(scriptEntry);

    const char* label = choose ? "Select this script" : "Close";
    button = Button::Create(label);
    button->GetSignal(Button::OnLeftClick).Connect( [this] { this->selectPressed = true; });
    vertBox->Pack(button, false, false);

    window->Add(vertBox);
}

string ScriptEditorWindow::getScript() {
    parent->SetState(Widget::State::INSENSITIVE);
    desktop.Add(window);

    while (sfWindow.isOpen()) {
        sf::Event event;
        while (sfWindow.pollEvent(event)) {
            desktop.HandleEvent(event);

            if (event.type==sf::Event::Closed)
                sfWindow.close();
        }
        desktop.Update(30/1000);

        if (openPressed) {
            openPressed = false;
            openScript();
        }
        if (savePressed) {
            savePressed = false;
            saveScript();
        }
        if (validatePressed) {
            validatePressed = false;
            showmessage(desktop, window, "Validation Result", validateScript());
        }
        if (selectPressed)
            break;

        desktop.BringToFront(window);
        sfWindow.clear();
        sfgui.Display(sfWindow);
        sfWindow.display();
        sf::sleep(sf::milliseconds(30));
    }

    saveScript();
    desktop.Remove(window);
    parent->SetState(Widget::State::NORMAL);

    if (loadedFile == fileEntry->GetText() && File::getExtension(loadedFile) == Properties::ScriptExtension)
        return loadedFile;
    return scriptEntry->GetText();
}

void ScriptEditorWindow::openScript() {
    saveScript();

    FilePicker picker(desktop, window, "Open Script", Properties::ScriptPath, Properties::ScriptExtension);
    if (picker.pickFile()) {
        loadedFile = picker.getChoice();
        ifstream file(string(Properties::ScriptPath+loadedFile).c_str());
        stringstream stream;
        stream << file.rdbuf();
        scriptEntry->SetText(stream.str());
        fileEntry->SetText(loadedFile);
    }
}

void ScriptEditorWindow::saveScript() {
    string filename = fileEntry->GetText();
    if (loadedFile != filename && File::exists(Properties::ScriptPath+filename)) {
        if (!yesnobox(desktop, window, "Overwrite?", filename+" already exists, overwrite?"))
            return;
        loadedFile = filename;
    }

    if (validateScript() != NOERROR) {
        if (!yesnobox(desktop, window, "Errors Exist", "Errors present, save anyways?"))
            return;
    }

    ofstream file(string(Properties::ScriptPath+filename).c_str());
    file << string(scriptEntry->GetText());
}

string ScriptEditorWindow::validateScript() {
    string script = scriptEntry->GetText();
    if (File::getExtension(script) == Properties::ScriptExtension) {
        loadedFile = script;
        fileEntry->SetText(script);
        ifstream file(string(Properties::ScriptPath+script).c_str());
        stringstream stream;
        stream << file.rdbuf();
        scriptEntry->SetText(stream.str());
        script = stream.str();
    }

    //TODO - create script object and dryRun

    return NOERROR;
}
