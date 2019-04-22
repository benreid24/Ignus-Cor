#ifndef ANIMATION_EDITOR_HPP
#define ANIMATION_EDITOR_HPP

#include <SFGUI/Widgets.hpp>

/**
 * Tab for the animation editor. Invokes separate Python program that is full editor
 *
 * \ingroup Editor
 */
class AnimationEditor {
    sfg::Desktop& desktop;
	sfg::Notebook::Ptr owner;
	sfg::Box::Ptr container;

	/**
	 * Shows the files present in the animations folder
	 */
    void showFiles();

    /**
     * Opens the animation editor
     */
    void openEditor();

public:
    /**
     * Constructs GUI elements for the editor
     */
    AnimationEditor(sfg::Desktop& dk, sfg::Notebook::Ptr parent);

    /**
     * Updates the GUI
     */
    void update();
};

#endif // ANIMATION_EDITOR_HPP
