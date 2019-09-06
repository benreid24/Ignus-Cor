#include "Shared/Media/Animation.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Util/ResourcePool.hpp"
#include "Shared/Properties.hpp"
#include <iostream>
using namespace std;
using namespace sf;

Clock Animation::clock;

AnimationSource::AnimationSource()
{
    loop = true;
}

AnimationSource::AnimationSource(const string& file)
{
    load(file);
}

AnimationSource::~AnimationSource()
{
    //dtor
}

void AnimationSource::load(const string& file)
{
    File input(file);
    AnimationFrame temp;
    int maxL = 0;
    string path = File::getPath(file);

    spriteSheetFile = input.getString();
    if (FileExists(path+spriteSheetFile))
		path += spriteSheetFile;
	else
		path = Properties::SpriteSheetPath+spriteSheetFile;
    sheet = imagePool.loadResource(path);
    loop = bool(input.get<uint8_t>());
    int numFrames = input.get<uint16_t>();
    frames.resize(numFrames);
    for (int i = 0; i<numFrames; ++i)
    {
        temp.length = input.get<uint32_t>();
        int n = input.get<uint16_t>();
        for (int j = 0; j<n; ++j)
        {
        	temp.sourcePos.x = input.get<uint32_t>();
			temp.sourcePos.y = input.get<uint32_t>();
			temp.size.x = input.get<uint32_t>();
			temp.size.y = input.get<uint32_t>();
			temp.scaleX = double(input.get<uint32_t>())/100;
			temp.scaleY = double(input.get<uint32_t>())/100;
			temp.renderOffset.x = input.get<int32_t>();
			temp.renderOffset.y = input.get<int32_t>();
			temp.rotation = input.get<uint32_t>();
			temp.alpha = input.get<uint8_t>();
			frames[i].push_back(temp);
        }
        if (n>maxL)
			maxL = n;
    }
    sprites.reserve(maxL);
}

bool AnimationSource::isLooping() const
{
    return loop;
}

const std::vector<sf::Sprite>& AnimationSource::getFrame(unsigned int i, Vector2f pos, bool centerOrigin)
{
    if (i>=frames.size()) {
        return sprites;
    }

	sprites.resize(frames[i].size());
	for (unsigned int j = 0; j<frames[i].size(); ++j)
    {
    	Sprite sp;
    	sp.setTexture(*sheet,true);
    	sp.setTextureRect(IntRect(frames[i][j].sourcePos.x, frames[i][j].sourcePos.y, frames[i][j].size.x, frames[i][j].size.y));
		sp.setOrigin(frames[i][j].size.x/2,frames[i][j].size.y/2);
		sp.setColor(Color(255,255,255,frames[i][j].alpha));
		sp.setScale(frames[i][j].scaleX,frames[i][j].scaleY);
		sf::Vector2f offset = Vector2f(sp.getGlobalBounds().width/2,sp.getGlobalBounds().height/2) - sp.getOrigin();
		if (!centerOrigin)
            offset = -sp.getOrigin();
		sp.setRotation(frames[i][j].rotation);
		sp.setPosition(pos+frames[i][j].renderOffset-offset);
		sprites[j] = sp;
    }
    return sprites;
}

sf::Vector2f AnimationSource::getFrameSize(unsigned int n) {
    if (n>=frames.size())
        return Vector2f(0,0);

    sf::FloatRect bounds(0, 0, 0, 0); //width/height = right/bottom
    const vector<Sprite>& pieces = getFrame(n, Vector2f(0,0), false);

    for (unsigned int i = 0; i<pieces.size(); ++i) {
        FloatRect pb = pieces[i].getGlobalBounds();
        if (pb.left < bounds.left)
            bounds.left = pb.left;
        if (pb.top < bounds.top)
            bounds.top = pb.top;
        if (pb.left + pb.width > bounds.width)
            bounds.width = pb.left + pb.width;
        if (pb.top + pb.height > bounds.height)
            bounds.height = pb.top + pb.height;
    }

    return sf::Vector2f(bounds.width - bounds.left, bounds.height - bounds.top);
}

unsigned int AnimationSource::incFrame(unsigned int cFrm, int lTime)
{
    if (cFrm>=frames.size()) {
        return 0;
    }

	if (frames[cFrm].size()==0) //current frame is empty, go to next
	{
		if (cFrm+1<frames.size())
			return cFrm+1;
		return cFrm;
	}

    if (Animation::clock.getElapsedTime().asMilliseconds()-lTime>=frames[cFrm][0].length)
    {
        if (cFrm+1<frames.size())
            return cFrm+1;
        else if (loop)
            return 0;
        else
            return cFrm;
    }

    return cFrm;
}

unsigned int AnimationSource::numFrames() const
{
    return frames.size();
}

string AnimationSource::getSpritesheetFilename() {
	return spriteSheetFile;
}

Animation::Animation()
{
    curFrm = lastFrmChangeTime = 0;
    playing = false;
    isCenterOrigin = false;
    looping = false;
}

Animation::Animation(AnimationReference ref, bool centerOrigin) : Animation()
{
    isCenterOrigin = centerOrigin;
    animSrc = ref;
    looping = ref->isLooping();
}

Animation::~Animation()
{
    //dtor
}

void Animation::setSource(AnimationReference src)
{
    animSrc = src;
    curFrm = 0;
    lastFrmChangeTime = Animation::clock.getElapsedTime().asMilliseconds();
    looping = animSrc->isLooping();
}

void Animation::update()
{
    if (!animSrc)
        return;

    unsigned int t = curFrm;
    if (playing || isLooping())
        curFrm = animSrc->incFrame(curFrm,lastFrmChangeTime);
    if (t!=curFrm)
        lastFrmChangeTime = Animation::clock.getElapsedTime().asMilliseconds();

    if (curFrm==animSrc->numFrames()-1 && playing)
    {
        if (isLooping())
            setFrame(0);
        else
            playing = false;
    }
}

void Animation::setFrame(unsigned int frm)
{
    curFrm = frm;
    lastFrmChangeTime = Animation::clock.getElapsedTime().asMilliseconds();
    playing = false;
}

unsigned int Animation::getCurrentFrame() const {
    return curFrm;
}

bool Animation::finished() const
{
    if (!animSrc)
        return false;

    return (!isLooping() && curFrm==animSrc->numFrames()-1) || animSrc->numFrames()==1;
}

Vector2f Animation::getSize() const {
	Vector2f zero(0,0);
	if (!animSrc)
        return zero;
	vector<Sprite> frames = animSrc->getFrame(0, zero, isCenterOrigin);
	if (frames.size()==0)
		return zero;
	return Vector2f(frames[0].getGlobalBounds().width, frames[0].getGlobalBounds().height);
}

bool Animation::isLooping() const
{
    return looping;
}

void Animation::setLooping(bool loop) {
    looping = loop;
}

void Animation::play()
{
    if (!animSrc)
        return;

    if (!isLooping())
    {
        setFrame(0);
        playing = true;
    }
}

void Animation::setPosition(Vector2f pos)
{
    position = pos;
}

void Animation::draw(sf::RenderTarget& window)
{
    if (!animSrc)
        return;

    update();
    const std::vector<Sprite>& t = animSrc->getFrame(curFrm, position, isCenterOrigin);
    for (unsigned int i = 0; i<t.size(); ++i)
		window.draw(t[i]);
}

bool Animation::isPlaying() const {
    return playing;
}
