#ifndef FILEIMPORTER_HPP
#define FILEIMPORTER_HPP

#include "Editor/Helpers/FilePicker.hpp"
#include <vector>
#include <string>

/**
 * Helper class to import or pick files
 *
 * \ingroup Editor
 */
class FileImporter {
public:
    enum FileType {
        Image,
        Animation,
        Audio,
        Music
    };

    /**
     * Creates the importer from the given in game file path and file type
     */
    FileImporter(const std::string& pickerPath, FileType filetype);

    /**
     * Creates the importer from the given in game file path and file extension to search for
     */
    FileImporter(const std::string& pickerPath, const std::string& extension);

    /**
     * Runs the file importer and returns the chosen file path relative to the given search path
     */
    std::string importFile(sfg::Desktop& desktop, sfg::Widget::Ptr parent);

private:
    std::string resourceDir;
    std::string extension;
};

#endif // FILEIMPORTER_HPP
