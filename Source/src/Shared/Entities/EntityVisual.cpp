#include "Shared/Entities/EntityVisual.hpp"
#include "Shared/Util/File.hpp"
using namespace std;
using namespace sf;

EntityVisual::EntityVisual() {
	type = NotLoaded;
	state = Still;
	dir = 0;
}

void EntityVisual::load(std::string path1, std::string path2) {
	if (File::getExtension(path1)==path1 || path1[path1.size()-1]=='/' || path1[path1.size()-1]=='\\') {
		if (path1[path1.size()-1]!='/' && path1[path1.size()-1]!='\\')
			path1 += "/";
		type = MoveAnim;
		slowSrc[0] = animPool.loadResource(path1+"up.anim");
		slowSrc[1] = animPool.loadResource(path1+"right.anim");
		slowSrc[2] = animPool.loadResource(path1+"down.anim");
		slowSrc[3] = animPool.loadResource(path1+"left.anim");
	}
}
