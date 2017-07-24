#ifndef FILE_HPP
#define FILE_HPP

#include <SFML/System.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

/**
 * Utility class to load and read binary data files
 *
 * \ingroup Utilities
 */
class File : private sf::NonCopyable
{
    std::fstream file;

    public:
    /**
     * Defines the mode of the file
     */
    enum OpenMode
    {
        In,
        Out
    };

    /**
     * Creates an empty file object with no data
     */
    File();

    /**
     * Creates and loads the given file in the given mode
     *
     * \param name The path to the file to open
     * \param mode The mode to open the file in
     */
    File(std::string name, OpenMode mode = In);

    /**
     * Closes the file
     */
    ~File();

    /**
     * Creates and loads the given file in the given mode
     *
     * \param name The path to the file to open
     * \param mode The mode to open the file in
     */
    void setFile(std::string name, OpenMode mode= In);

    /**
     * Closes the file
     */
    void close();

    /**
     * Writes the given data type to the file byte by byte starting with the first byte
     *
     * \param data The data to write
     */
    template <typename T>
    void write(const T& data)
    {
        if (file.good())
        {
            uint8_t bytes[sizeof(T)];
            for(unsigned int i = 0; i < sizeof(T); ++i)
                bytes[i] = static_cast<uint8_t>( data >> (i*8) );
            file.write((char*)bytes, sizeof(T));
        }
    }

    /**
     * Writes the string to the file by first writing a uint32_t containing the size, followed by the string itself as single byte characters
     *
     * \param str The string to write
     */
    void writeString(std::string str)
    {
        if (file.good())
        {
            write<uint32_t>(str.size());
            file.write(str.c_str(),str.size());
        }
    }

    /**
     * Same as write, but for an array
     *
     * \param data The array to write
     * \param size The amount of elements to write
     */
    template <typename T>
    void writeArray(T* data, int size)
    {
        for (int i = 0; i<size; ++i)
            write<T>(data[i]);
    }

    /**
     * Reads the given data type from the file
     */
    template <typename T>
    T get()
    {
        if (file.good())
        {
            T v = 0;
            uint8_t bytes[sizeof(T)];

            file.read((char*)bytes, sizeof(T));
            for(unsigned int i = 0; i < sizeof(T); ++i)
                v |= static_cast<T>(bytes[i]) << (i*8);

            return v;
        }
        return 0;
    }

    /**
     * Reads a string from the file
     */
    std::string getString()
    {
        if (file.good())
        {
            int size = get<uint32_t>();
            if (!file.good())
				return "";
            char* buf = new char[size+1];
            file.read(buf,size);
            std::string ret(buf,size);
            delete[] buf;
            return ret;
        }
        else
            return "";
    }

    /**
     * Reads an array from the file
     *
     * \param data The array to put the loaded data into
     * \param size The amount of elements to read
     */
    template <typename T>
    void getArray(T* data, int size)
    {
        for (int i = 0; i<size; ++i)
            data[i] = get<T>();
    }

    /**
     * Tells whether or not the file is still ok to read or write to
     */
	bool good()
	{
		return file.good();
	}

    /**
     * Returns the extension of a filename
     *
     * \param file The filename to parse for the extension
     * \return The extension of the file
     */
    static std::string getExtension(std::string file);

    /**
     * Returns the base name of a file by removing the path and extension
     *
     * \param file The filename to parse
     * \return The base name of the file
     */
    static std::string getBaseName(std::string file);

    /**
     * Returns the path of a given filename
     *
     * \param file The filename
     * \return The path of the passed file
     */
	static std::string getPath(std::string file);
};

/**
 * Tells whether or not the given file exists
 *
 * \param filename The file to check
 * \return Whether or not the file exists
 *
 * \ingroup Utilities
 */
bool FileExists(std::string filename);

/**
 * Copies the given file to the given destination
 *
 * \param src The file to copy from
 * \param dest The file to copy to
 */
void copyFile(std::string src, std::string dest);

/**
 * Returns a file listing of the given directory
 *
 * \param dir The directory to search
 * \param ext The file extension
 * \param inclSubdirs Whether or not to recursively search
 * \return A vector containing the filenames of all the files that matched
 */
std::vector<std::string> listDirectory(std::string dir, std::string ext, bool inclSubdirs);

#ifdef EDITOR
/**
 * Helper function to open the Window file dialogue window to get a file
 *
 * \param f The file extension to look for
 * \param s True if this is a file being saved
 * \param c True if this is a file being created
 */
std::string getFilename(const char* f, bool s, bool c);

/**
 * Helper function to open the Window file dialogue window to get a folder
 *
 * \param f The file extension to look for
 * \param s True if this is a file being saved
 * \param c True if this is a file being created
 */
std::string getFoldername();
#endif

#endif
