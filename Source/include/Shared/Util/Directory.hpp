#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <string>
#include <vector>
#include <memory>

/**
 * Helper class that represents a directory on the file system
 *
 * \ingroup Utilities
 */
class Directory {
public:
    typedef std::shared_ptr<Directory> Ptr;

private:
    Ptr parent;
    std::string name;
    std::vector<Ptr> subDirs;
    std::vector<std::string> files;

    Directory(std::string name, Ptr parent);

    static Ptr p_get(std::string name, std::string path, std::string ext, Ptr parent);

public:

    /**
     * Create a Directory from the file system with an optional file type filter
     */
    static Ptr get(std::string path, std::string extension = "");

    /**
     * Builds the full pathname of a file
     */
    static std::string buildPath(Ptr dir, std::string file);

    /**
     * Returns the name
     */
    std::string getName();

    /**
     * Returns the list of files in the Directory
     */
    std::vector<std::string> getFiles();

    /**
     * Returns the list of sub directories in the Directory
     */
    std::vector<Ptr> getSubDirectories();

    /**
     * Returns a pointer to the parent Directory or nullptr if none
     */
    Ptr getParent();

    /**
     * Whether or not this Directory has a parent
     */
    bool hasParent();
};

#endif // DIRECTORY_HPP
