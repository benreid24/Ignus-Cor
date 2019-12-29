#include "Shared/Particles/ParticleGraphics.hpp"
#include "Shared/Util/File.hpp"
#include "Shared/Properties.hpp"
using namespace std;
using namespace sf;

ParticleGraphics::ParticleGraphics(const string& file) : ParticleGraphics() {
    load(file);
}

void ParticleGraphics::load(const string& file) {
    if (File::getExtension(file)=="anim") {
        gfxType = Anim;
        if (!File::exists(file))
            animRef = animPool.loadResource(Properties::ParticleAnimPath + file);
        else
            animRef = animPool.loadResource(file);
        animation.setSource(animRef);
    }
    else if (File::getExtension(file)=="png") {
        gfxType = Image;
        if (!File::exists(file))
            texture = imagePool.loadResource(Properties::ParticleImagePath + file);
        else
            texture = imagePool.loadResource(file);
        sprite.setTexture(*texture);
        sprite.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
    }
    else if (file.size()>0)
        cout << "Warning: Failed to load EntityGraphics file '" << file << "'\n";
}

void ParticleGraphics::render(RenderTarget& target, Vector2f pos, int rotation, int opacity) {
    switch (gfxType) {
        case Anim:
            animation.setPosition(pos);
            animation.draw(target);
            break;

        case Image:
            sprite.setPosition(pos);
            sprite.setRotation(rotation);
            sprite.setColor(Color(255,255,255,opacity));
            target.draw(sprite);
            break;

        default:
            break;
    }
}

bool ParticleGraphics::finished() {
    return animation.finished();
}
