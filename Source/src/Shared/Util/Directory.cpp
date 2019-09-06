#include "Shared/Util/Directory.hpp"
#include "Shared/Util/File.hpp"
#include <dirent.h>
using namespace std;

Directory::Directory(string nm, Directory::Ptr p) {
    name = nm;
    parent = p;
}

Directory::Ptr Directory::p_get(string name, string dir, string ext, Directory::Ptr parent) {
    Directory::Ptr folder(new Directory(name, parent));

    DIR* cDir;
    struct dirent* cFile;

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
                if (tmp.size()>2 && (File::getExtension(tmp)==ext || ext.size()==0))
					folder->files.push_back(tmp);
            }
            else
				folder->subDirs.push_back(p_get(tmp, dir+tmp, ext, folder));
        }
    }
    return folder;
}

Directory::Ptr Directory::get(string path, string ext) {
    return p_get("Root", path, ext, nullptr);
}

string Directory::buildPath(Directory::Ptr dir, string name) {
    while (dir->hasParent()) {
        name.insert(0, dir->getName()+"/");
        dir = dir->getParent();
    }
    return name;
}

string Directory::getName() {
    return name;
}

vector<string> Directory::getFiles() {
    return files;
}

vector<Directory::Ptr> Directory::getSubDirectories() {
    return subDirs;
}

Directory::Ptr Directory::getParent() {
    return parent;
}

bool Directory::hasParent() {
    return parent.get() != nullptr;
}
