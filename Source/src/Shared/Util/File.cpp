#include "Shared/Util/File.hpp"
#include <iostream>
#include <algorithm>
using namespace std;

bool FileExists(std::string filename)
{
    std::ifstream file(filename.c_str());
    return file.good();
}

void copyFile(std::string src, std::string dest) {
    ifstream source(src.c_str(), ios::binary);
    ofstream dst(dest.c_str(), ios::binary);

    istreambuf_iterator<char> begin_source(source);
    istreambuf_iterator<char> end_source;
    ostreambuf_iterator<char> begin_dest(dst);
    copy(begin_source, end_source, begin_dest);
}

File::File()
{
    //ctor
}

File::File(std::string name, OpenMode mode)
{
    setFile(name, mode);
}

File::~File()
{
    file.close();
}

void File::setFile(std::string name, OpenMode mode)
{
    if (file.is_open())
        file.close();

    if (mode==In)
        file.open(name.c_str(), std::ios::in|std::ios::binary);
    else
    {
        FileExists(name); //ensure file exists
        file.open(name.c_str(), std::ios::binary | std::ios::out);
    }

    if (!file.good())
        std::cout << "Failed to open datafile: " << name << '\n';
}

void File::close()
{
    file.close();
}

std::string File::getExtension(std::string file)
{
    std::string ret;
    for (unsigned int i = 0; i<file.size(); ++i)
    {
        if (file[i]=='.')
            ret.clear();
        else
            ret.push_back(file[i]);
    }
    return ret;
}

std::string File::getBaseName(std::string file)
{
    std::string ret;
    for (int i = file.size()-1; i>=0; --i)
    {
        if (file[i]=='.')
            ret.clear();
        else if (file[i]!='/' && file[i]!='\\')
            ret.push_back(file[i]);

        if (file[i]=='/' || file[i]=='\\')
            break;
    }
    for (unsigned int i = 0; i<ret.size()/2; ++i)
        std::swap(ret[i],ret[ret.size()-i-1]);
    return ret;
}

std::string File::getPath(std::string file)
{
	std::string ret, temp;
	for (unsigned int i = 0; i<file.size(); ++i)
	{
		if (file[i]=='/')
		{
			ret += temp+"/";
			temp.clear();
		}
		else
			temp.push_back(file[i]);
	}
	return ret;
}

#ifdef EDITOR

#include <windows.h>

/**
 * Helper function to open the Window file dialogue window
 *
 * \param f The file extension to look for
 * \param s True if this is a file being saved
 * \param c True if this is a file being created
 */
std::string getFilename(const char* f, bool s, bool c)
{
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = f;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
    if (c)
        ofn.Flags = ofn.Flags | OFN_CREATEPROMPT;
    else
        ofn.Flags = ofn.Flags | OFN_FILEMUSTEXIST;
    ofn.lpstrDefExt = "";

    std::string fileNameStr;
    if (s)
    {
        if ( GetSaveFileName(&ofn) )
            fileNameStr = fileName;
    }
    else
    {
        if ( GetOpenFileName(&ofn) )
            fileNameStr = fileName;
    }

    return fileNameStr;
}

#endif // EDITOR
