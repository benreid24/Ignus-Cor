#include "Shared/Util/File.hpp"
#include <iostream>
#include <algorithm>
#include <dirent.h>
#include <direct.h>
using namespace std;

bool File::exists(const string& filename)
{
    ifstream file(filename.c_str());
    return file.good();
}

void File::copy(const string& src, const string& dest) {
    if (src == dest)
        return;

    ifstream source(src.c_str(), ios::binary);
    ofstream dst(dest.c_str(), ios::binary);

    istreambuf_iterator<char> begin_source(source);
    istreambuf_iterator<char> end_source;
    ostreambuf_iterator<char> begin_dest(dst);
    std::copy(begin_source, end_source, begin_dest);
}

File::File()
{
    //ctor
}

File::File(const string& name, OpenMode mode)
{
    setFile(name, mode);
}

File::~File()
{
    file.close();
}

void File::setFile(const string& name, OpenMode mode)
{
    if (file.is_open())
        file.close();

    if (mode==In)
        file.open(name.c_str(), ios::in|ios::binary);
    else
    {
        File::exists(name); //ensure file exists
        file.open(name.c_str(), ios::binary | ios::out);
    }

    if (!file.good())
        cout << "Failed to open datafile: " << name << '\n';
}

void File::close()
{
    file.close();
}

string File::getExtension(const string& file)
{
    string ret;
    for (unsigned int i = 0; i<file.size(); ++i)
    {
        if (file[i]=='.')
            ret.clear();
        else
            ret.push_back(file[i]);
    }
    return ret;
}

string File::getBaseName(const string& file)
{
    string ret;
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
        swap(ret[i],ret[ret.size()-i-1]);
    return ret;
}

string File::getPath(const string& file)
{
	string ret, temp;
	for (unsigned int i = 0; i<file.size(); ++i)
	{
		if (file[i]=='/' || file[i]=='\\')
		{
			ret += temp+"/";
			temp.clear();
		}
		else
			temp.push_back(file[i]);
	}
	return ret;
}

string File::stripPath(const string& file) {
    return getBaseName(file) + "." + getExtension(file);
}

vector<string> File::listDirectory(string dir, const string& ext, bool inclSubdirs) {
    DIR* cDir;
    struct dirent* cFile;
    vector<string> total;

    if (dir[dir.size()-1]!='/' && dir[dir.size()-1]!='\\')
		dir.push_back('/');

    cDir = opendir(dir.c_str());
    if (cDir!=nullptr)
    {
        while ((cFile = readdir(cDir)))
        {
            string tmp = cFile->d_name;
            if (tmp.find(".")!=string::npos)
            {
                if (tmp.size()>2 && File::getExtension(tmp)==ext)
					total.push_back(dir+tmp);
            }
            else if (inclSubdirs) {
                vector<string> files = File::listDirectory(dir+tmp,ext,true);
				total.insert(total.end(), files.begin(), files.end());
            }
        }
    }
    return total;
}

void File::createDirectories(const string& dir) {
    string cd;
    cd.reserve(dir.size());
    for (unsigned int i = 0; i<dir.size(); ++i) {
        if (dir[i] == '/' || dir[i] == '\\')
            _mkdir(cd.c_str());
        cd.push_back(dir[i]);
    }
}

#ifdef EDITOR

#include <windows.h>
#include <shlobj.h>

string File::getFile(const string& name, const string& ext, bool s, bool c)
{
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";

    // "Animation (*.anim)\0 *.anim\0\0"
    string filter = name + " (*." + ext + ")";
    filter.push_back('\0');
    filter += " *."+ ext;
    filter.push_back('\0');
    filter.push_back('\0');

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = filter.c_str();
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
    if (c)
        ofn.Flags = ofn.Flags | OFN_CREATEPROMPT;
    else
        ofn.Flags = ofn.Flags | OFN_FILEMUSTEXIST;
    ofn.lpstrDefExt = "";

    string fileNameStr;
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

string File::getFolder() {
	BROWSEINFO binf = {0};
	TCHAR path[MAX_PATH];
	binf.ulFlags |= BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pid = SHBrowseForFolder(&binf);
	if (pid==0)
		return "";
	SHGetPathFromIDList ( pid, path );

	IMalloc* imalloc = 0;
	if (SUCCEEDED(SHGetMalloc(&imalloc)))
	{
		imalloc->Free (pid);
		imalloc->Release ( );
	}
	return path;
}

#endif // EDITOR
