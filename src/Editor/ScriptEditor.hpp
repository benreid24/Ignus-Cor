#ifndef SCRIPTEDITOR_HPP
#define SCRIPTEDITOR_HPP

#include <SFGUI/Widgets.hpp>
#include "Editor/Helpers/FilePicker.hpp"

/**
 * Editor tab for editing scripts
 *
 * \ingroup Editor
 */
class ScriptEditor {
public:
    /**
     * Creates the editor window
     */
    ScriptEditor(sfg::Desktop& dk, sfg::Notebook::Ptr owner);

    /**
     * Updates the editor window
     */
    void update();

private:
    sfg::Desktop& desktop;
    sfg::Notebook::Ptr owner;
    sfg::Box::Ptr container;
    FilePicker* picker;
    bool newScript, delScript, editScript;
};

#endif // SCRIPTEDITOR_HPP
