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
		type = MoveAnim;
		if (path1[path1.size()-1]!='/' && path1[path1.size()-1]!='\\')
			path1 += "/";
		type = MoveAnim;
		slowSrc[0] = animPool.loadResource(path1+"up.anim");
		slowSrc[1] = animPool.loadResource(path1+"right.anim");
		slowSrc[2] = animPool.loadResource(path1+"down.anim");
		slowSrc[3] = animPool.loadResource(path1+"left.anim");
		for (int i = 0; i<4; ++i)
			slow[i].setSource(slowSrc[i]);
	}
	else {
		if (File::getExtension(path1)=="png") {
			type = StaticImage;
			txtr = imagePool.loadResource(path1);
			image.setTexture(*txtr);
		}
		else {
			type = SingleAnim;
			slowSrc[0] = animPool.loadResource(path1);
			slow[0].setSource(slowSrc[0]);
		}
	}
	if (path2.size()>0) {
		type = SpeedAnim;
		if (path2[path2.size()-1]!='/' && path2[path2.size()-1]!='\\')
			path2 += "/";
		type = MoveAnim;
		fastSrc[0] = animPool.loadResource(path2+"up.anim");
		fastSrc[1] = animPool.loadResource(path2+"right.anim");
		fastSrc[2] = animPool.loadResource(path2+"down.anim");
		fastSrc[3] = animPool.loadResource(path2+"left.anim");
		for (int i = 0; i<4; ++i)
			fast[i].setSource(fastSrc[i]);
	}
}

void EntityVisual::setDirection(int d) {
	dir = d;
}

void EntityVisual::setMoving(int d, bool f) {
	dir = d;
	state = f?(FastMoving):(Moving);
	slow[dir].play();
	fast[dir].play();
}

Vector2f EntityVisual::getSize() {
	switch (type) {
	case StaticImage:
		return Vector2f(image.getGlobalBounds().width, image.getGlobalBounds().height);
	case MoveAnim:
	case SpeedAnim:
		return slow[0].getSize();
	default:
		cout << "Warning: EntityVisual getSize() called but type is invalid\n";
		return Vector2f(0,0);
	}
}

void EntityVisual::render(sf::RenderTarget& target, sf::Vector2f position) {
    for (int i = 0; i<4; ++i) {
		slow[i].setPosition(position);
		fast[i].setPosition(position);
		if (state==Still) {
			slow[i].setFrame(0);
			fast[i].setFrame(0);
		}
		else {
			slow[i].update();
			fast[i].update();
		}
    }
    image.setPosition(position);

    switch (type) {
	case StaticImage:
		target.draw(image);
		break;

	case MoveAnim:
		slow[dir].draw(target);
		break;

	case SpeedAnim:
		if (state==Moving)
			slow[dir].draw(target);
		else
			fast[dir].draw(target);
		break;

	default:
		break;
    }

    state = Still; //setMoving will be called before next render if entity is moving, so this is fine
}
