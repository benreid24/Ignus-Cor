#include "Editor/Helpers/FileImporter.hpp"
#include "Editor/Helpers/Dialogs.hpp"
#include "Shared/Media/Animation.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Properties.hpp"
#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
using namespace std;
using namespace sfg;
using namespace sf;

extern RenderWindow sfWindow;
extern SFGUI sfgui;

namespace {
bool animSpritesheetHelper(const string& animFile, const string& ogDir) {
    AnimationSource anim(animFile);
    if (!anim.spritesheetFound()) {
        if (File::exists(ogDir+anim.getSpritesheetFilename())) {
            File::copy(ogDir+anim.getSpritesheetFilename(), File::getPath(animFile)+anim.getSpritesheetFilename());
            return true;
        }
        else {
            string sheet = File::getFile(anim.getSpritesheetFilename(), "png", false, false);
            if (!sheet.empty()) {
                File::copy(sheet, File::getPath(animFile)+anim.getSpritesheetFilename());
                return true;
            }
            return false;
        }
    }
    return true;
}
}

FileImporter::FileImporter(const string& path, const string& ext)
: resourceDir(path), extension(ext) {}

FileImporter::FileImporter(const string& path, FileType type)
: resourceDir(path) {
    switch (type) {
    case Image:
        extension = "png";
        break;
    case Animation:
        extension = "anim";
        break;
    case Audio:
        extension = "wav";
        break;
    case Music:
        extension = "ogg";
        break;
    default:
        extension = "png";
    }
}

string FileImporter::importFile(Desktop& desktop, Widget::Ptr parent) {
    string chosen;

    parent->SetState(Widget::State::INSENSITIVE);
    sfg::Window::Ptr window = sfg::Window::Create();
    window->SetTitle("Import " + extension + " file");
    Box::Ptr winBox = Box::Create(Box::Orientation::HORIZONTAL,5);
    window->Add(winBox);
    desktop.Add(window);

    bool pickFile = false;
    Button::Ptr pickBut = Button::Create("Pick Existing");
    pickBut->GetSignal(Button::OnLeftClick).Connect( [&pickFile] { pickFile = true; } );
    bool importFile = false;
    Button::Ptr importBut = Button::Create("Import New");
    importBut->GetSignal(Button::OnLeftClick).Connect( [&importFile] { importFile = true; } );
    bool cancel = false;
    Button::Ptr cancelBut = Button::Create("Cancel");
    cancelBut->GetSignal(Button::OnLeftClick).Connect( [&cancel] { cancel = true; } );
    winBox->Pack(pickBut,false,false);
    winBox->Pack(importBut,false,false);
    winBox->Pack(cancelBut,false,false);

    while (sfWindow.isOpen()) {
        Event wv;
        while (sfWindow.pollEvent(wv)) {
            desktop.HandleEvent(wv);

            if (wv.type==Event::Closed)
                sfWindow.close();
        }
        desktop.Update(30/1000);

        if (cancel)
            break;
        if (pickFile) {
            pickFile = false;
            FilePicker picker(desktop, window, "Pick Existing", resourceDir, extension, false);
            if (picker.pickFile()) {
                chosen = picker.getChoice();
                break;
            }
        }
        if (importFile) {
            importFile = false;
            string file = File::getFile(extension, extension, false, false);
            if (!file.empty()) {
                chosen = File::stripPath(file);
                File::copy(file, resourceDir+chosen);
                if (extension == "anim") {
                    bool ok = animSpritesheetHelper(resourceDir+chosen, File::getPath(file));
                    while (!ok) {
                        if (!yesnobox(desktop, window, "Spritesheet Missing",
                            "Animation is missing spritesheet. If you do not import one the import will be canceled. Try again?"))
                            break;
                        ok = animSpritesheetHelper(resourceDir+chosen, File::getPath(file));
                    }
                    if (ok)
                        break;
                    else {
                        remove(string(resourceDir+chosen).c_str());
                        chosen.clear();
                    }
                }
                else
                    break;
            }
        }

        desktop.BringToFront(window);
        sfWindow.clear();
        sfgui.Display(sfWindow);
        sfWindow.display();
        sleep(milliseconds(30));
    }

    desktop.Remove(window);
    parent->SetState(Widget::State::NORMAL);
    return chosen;
}
