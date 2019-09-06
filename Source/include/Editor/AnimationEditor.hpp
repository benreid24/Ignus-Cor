#ifndef ANIMATION_EDITOR_HPP
#define ANIMATION_EDITOR_HPP

#include <SFGUI/Widgets.hpp>
#include "Editor/Helpers/FilePicker.hpp"

/**
 * Tab for the animation editor. Invokes separate Python program that is full editor
 *
 * \ingroup Editor
 */
class AnimationEditor {
    sfg::Desktop& desktop;
	sfg::Notebook::Ptr owner;
	sfg::Box::Ptr container;
	FilePicker* fileList;
	bool refreshOnUpdate;

    void openEditor();
    void refresh();

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

#endif // ANIMATION_EDITOR_HPP
