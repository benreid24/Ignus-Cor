#ifndef SCRIPTEDITORWINDOW_HPP
#define SCRIPTEDITORWINDOW_HPP

#include <SFGUI/Widgets.hpp>
#include "Editor/GUI/MultiLineEntry.hpp"

/**
 * Creates a window for picking existing scripts, creating new scripts, and editing scripts.
 * Also provides validation
 *
 * \ingroup Editor
 */
class ScriptEditorWindow {
public:
    /**
     * Creates the window and optionally loads the given script
     */
    ScriptEditorWindow(sfg::Desktop& dk, sfg::Widget::Ptr parent,
                       const std::string& script = "", bool choose = true);

    /**
     * Performs the actual script selection/editing. Returns the selected/created script
     */
    std::string getScript();

private:
    sfg::Desktop& desktop;
    sfg::Widget::Ptr parent;
    sfg::Window::Ptr window;
    sfg::MultiLineEntry::Ptr scriptEntry;
    sfg::Entry::Ptr fileEntry;
    std::string loadedFile; //determine if overwriting different file
    bool savePressed, openPressed, validatePressed, selectPressed;
    bool dirty;

    /**
     * Performs validation of the currently loaded script and returns any errors
     */
    std::string validateScript();

    /**
     * Saves the script to the given file (if valid)
     */
    void saveScript();

    /**
     * Creates a file picker and loads a new script if one is chosen
     */
    void openScript();
};

#endif // SCRIPTEDITORWINDOW_HPP
