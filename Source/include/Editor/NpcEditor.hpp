#ifndef NPCEDITOR_HPP
#define NPCEDITOR_HPP

#include <SFGUI/Widgets.hpp>
#include "Editor/Helpers/FilePicker.hpp"

class NpcEditor {
    sfg::Desktop& desktop;
	sfg::Notebook::Ptr owner;
	sfg::Box::Ptr container;
	FilePicker* fileList;
	bool refreshOnUpdate;

	//

    void newNpc();
    void editNpc(string file);
    void deleteNpc();

public:
    /**
     * Constructs GUI elements for the editor
     */
    AnimationEditor(sfg::Desktop& dk, sfg::Notebook::Ptr parent);

    /**
     * Frees memory
     */
    ~AnimationEditor();

    /**
     * Updates the GUI
     */
    void update();
};

#endif // NPCEDITOR_HPP
