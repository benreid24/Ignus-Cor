#include "Shared/Entities/EntityVisual.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Properties.hpp"
using namespace std;
using namespace sf;

EntityVisual::EntityVisual() {
	type = NotLoaded;
	state = Still;
	dir = 0;
	setMoveCalled = false;
}

void EntityVisual::load(std::string path1, std::string path2) {
    if (path1.size()==0)
        return;

	if (File::getExtension(path1)==path1 || path1[path1.size()-1]=='/' || path1[path1.size()-1]=='\\') {
		type = MoveAnim;
		path1 = Properties::AnimationPath + path1;
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
            path1 = Properties::EntityImagePath + path1;
			type = StaticImage;
			txtr = imagePool.loadResource(path1);
			image.setTexture(*txtr);
		}
		else {
			type = SingleAnim;
			path1 = Properties::AnimationPath + path1;
			slowSrc[0] = animPool.loadResource(path1);
			slow[0].setSource(slowSrc[0]);
		}
	}
	if (path2.size()>0) {
		type = SpeedAnim;
		path2 = Properties::AnimationPath + path2;
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
	setMoveCalled = true;
	state = f?(FastMoving):(Moving);
	if (type==MoveAnim || type==SpeedAnim) {
        if (!slow[dir].isPlaying())
            slow[dir].play();
        if (!fast[dir].isPlaying())
            fast[dir].play();
	}
	else if (type == SingleAnim && !slow[0].isPlaying())
        slow[0].play();
}

Vector2f EntityVisual::getSize() const {
	switch (type) {
	case StaticImage:
		return Vector2f(image.getGlobalBounds().width, image.getGlobalBounds().height);
	case MoveAnim:
	case SpeedAnim:
    case SingleAnim:
		return slow[0].getSize();
    case NotLoaded:
        return Vector2f(0,0);
	default:
		cout << "Warning: EntityVisual getSize() called but type is invalid\n";
		return Vector2f(0,0);
	}
}

void EntityVisual::render(sf::RenderTarget& target, sf::Vector2f position) {
    for (int i = 0; i<4; ++i) {
		slow[i].setPosition(position);
		fast[i].setPosition(position);
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

    case SingleAnim:
        slow[0].draw(target);
        break;

	default:
		break;
    }
}

bool EntityVisual::animationDone() const {
    switch (type) {
        case MoveAnim:
        case SpeedAnim:
            switch (state) {
                case Moving:
                    return slow[dir].finished();
                case FastMoving:
                    return fast[dir].finished();
            }

        case SingleAnim:
            return slow[0].finished();
    }
    return false;
}

void EntityVisual::update() {
    if (!setMoveCalled)
        state = Still;
    setMoveCalled = false;

    for (int i = 0; i<4; ++i) {
		if (state==Still) {
			slow[i].setFrame(0);
			fast[i].setFrame(0);
		}
		else {
			slow[i].update();
			fast[i].update();
		}
    }
}
