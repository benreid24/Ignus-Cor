#ifndef CONVERSATIONEDITOR_HPP
#define CONVERSATIONEDITOR_HPP

#include <SFGUI/Widgets.hpp>
#include "Editor/Helpers/FilePicker.hpp"

/**
 * Editor tab for editing conversations
 *
 * \ingroup Editor
 */
class ConversationEditor {
public:
    /**
     * Creates the editor window
     */
    ConversationEditor(sfg::Desktop& dk, sfg::Notebook::Ptr owner);

    /**
     * Updates the editor window
     */
    void update();

private:
    sfg::Desktop& desktop;
    sfg::Notebook::Ptr owner;
    sfg::Box::Ptr container;
    FilePicker* picker;
    bool newConv, delConv, editConv;
};

#endif // CONVERSATIONEDITOR_HPP
