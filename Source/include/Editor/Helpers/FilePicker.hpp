#ifndef FILEPICKER_HPP
#define FILEPICKER_HPP

#include <string>
#include <map>
#include <vector>
#include <SFGUI/Widgets.hpp>

/**
 * Helper GUI class to provide a window for searching for files
 *
 * \ingroup Editor
 */
class FilePicker {
	std::string dispName;
	std::map<std::string,std::vector<std::string> > files; // [subdir] -> [full filename]
	std::string chosenFile;
	enum State {
		Picking,
		Chosen,
		Canceled
	}state;

	sfg::Desktop& owner;
	sfg::Widget::Ptr parent;
	sfg::Window::Ptr window;
	sfg::Box::Ptr container;
	sfg::Label::Ptr curFile;
	sfg::Button::Ptr pickButton, cancelButton;
	sfg::ScrolledWindow::Ptr subDirButtons;
	std::map<std::string,sfg::ScrolledWindow::Ptr> fileButtons;

	/**
	 * Call back for buttons to set the current file
	 */
	void setChoice(std::string file);

	/**
	 * Callback to set the current directory to display files for
	 */
	void setDirectory(std::string subDir);

	/**
	 * Sets the state. Used by cancel and pick buttons
	 */
	void setState(State s);

public:
    /**
     * Creates the FilePicker to search the given directory and subdirectories for files with the given extension
     */
	FilePicker(sfg::Desktop& desktop, sfg::Widget::Ptr parent, std::string dispName, std::string searchDir, std::string extension);

	/**
	 * Does the actual GUI stuff
	 *
	 * \return True if the user picked a file, false if they canceled
	 */
	bool pickFile();

	/**
	 * Returns the full path to the chosen file, or empty if none
	 */
	std::string getChoice();
};

#endif // FILEPICKER_HPP
