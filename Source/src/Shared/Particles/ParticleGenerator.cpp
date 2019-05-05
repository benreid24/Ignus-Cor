#include "Shared/Particles/ParticleGenerator.hpp"
#include "Shared/Util/Util.hpp"
using namespace std;
using namespace sf;

float ParticleGenerator::ValueWindow::getValue() {
    float range = maximum - minimum;
    float r = randomInt(0, 10000000);
    return minimum + r/10000000*range;
}

ParticleGenerator::ParticleGenerator(ParticleGenerator::ValueWindow radius, ParticleGenerator::ValueWindow angle,
                                     ParticleGenerator::ValueWindow speed, ParticleGenerator::ValueWindow dir)
: spawnRadius(radius), spawnAngle(angle), spawnVelocity(speed), spawnDirection(dir), spawnOpacity(255,255) {
    lifetimeType = UntilDestroyedLifetime;
    particlesPerSecond = 0;
    lastSpawnTime = 0;
}

ParticleGenerator::~ParticleGenerator() {
    for (auto i = particles.begin(); i!=particles.end(); ++i) {
        delete *i;
    }
}

void ParticleGenerator::setParticleBehaviors(Particle::Behavior rotBehave, Particle::Behavior velBehave, Particle::Behavior opacBehave) {
    rotationBehavior = rotBehave;
    velocityBehavior = velBehave;
    opacityBehavior = opacBehave;
}

void ParticleGenerator::setParticleLifetime(Particle::LifetimeType lifetimeType, float lifetimeValue, float fuzziness) {
    particleLifetimeType = lifetimeType;
    particleLifetimeValue = lifetimeValue;
    particleLifetimeFuzziness = fuzziness;
}

void ParticleGenerator::setParticleGraphics(const string& gfx) {
    defaultParticleGraphics.load(gfx);
}

void ParticleGenerator::setParticleOpacity(ParticleGenerator::ValueWindow opac) {
    spawnOpacity = opac;
}

void ParticleGenerator::setLifetime(LifetimeType type, float lifetime) {
    lifetimeType = type;
    lifetimeValue = lifetime;
}

void ParticleGenerator::setGenerationRate(float rate, GenerationRateChangeScale rateChgType, float rateChgMultiplier) {
    particlesPerSecond = rate;
    genChgType = rateChgType;
    genChgMultiplier = rateChgMultiplier;
}

void ParticleGenerator::stop(float stopTime) {
    genChgType = Constant;
    genChgMultiplier = -particlesPerSecond/stopTime;
}

bool ParticleGenerator::finished() {
    for (auto i = particles.begin(); i!=particles.end(); ++i) {
        if (!(*i)->finished(timer.getElapsedTime().asSeconds()) && (*i)->blocksGeneratorDestruction())
            return false;
    }
    return true;
}

void ParticleGenerator::update() {
    updateSpawnRate();
    spawnParticles();
    for (auto i = particles.begin(); i!=particles.end(); ++i) {
        (*i)->update(timer.getElapsedTime().asSeconds());
        if ((*i)->finished(timer.getElapsedTime().asSeconds())) {
            auto j = i--;
            delete *j;
            particles.erase(j);
        }
    }
}

void ParticleGenerator::render(RenderTarget& target, Vector2f pos) {
    for (auto i = particles.begin(); i!=particles.end(); ++i) {
        (*i)->render(target, pos);
    }
}

void ParticleGenerator::spawnParticles() {
    float dt = timer.getElapsedTime().asSeconds() - lastSpawnTime;
    int spawnCount = particlesPerSecond * dt;
    ValueWindow lifetimeWindow(particleLifetimeValue-particleLifetimeFuzziness, particleLifetimeValue+particleLifetimeFuzziness);

    for (int i = 0; i<=spawnCount; ++i) {
        float angle = spawnAngle.getValue() * 3.1415926/180;
        float rad = spawnRadius.getValue();
        float x = rad*cos(angle), y = rad*sin(angle);
        Particle* p = new Particle(defaultParticleGraphics, Vector2f(x,y), spawnDirection.getValue(),
                                   spawnVelocity.getValue(), spawnOpacity.getValue(), timer.getElapsedTime().asSeconds());
        p->setRotationBehavior(rotationBehavior);
        p->setVelocityBehavior(velocityBehavior);
        p->setOpacityBehavior(opacityBehavior);
        p->setLifetime(particleLifetimeType, lifetimeWindow.getValue());
        particles.push_back(p);
    }
}

void ParticleGenerator::updateSpawnRate() {
    float dt = timer.getElapsedTime().asSeconds();
    switch (genChgType) {
        case Constant:
            particlesPerSecond += genChgMultiplier;
            break;

        case Linear:
            particlesPerSecond += genChgMultiplier * dt;
            break;

        case Exponential:
            particlesPerSecond += genChgMultiplier * dt*dt;
            break;

        case Decay:
            particlesPerSecond += genChgMultiplier / dt;

        case ExponentialDecay:
            particlesPerSecond += genChgMultiplier / (dt*dt);
            break;

        default:
            particlesPerSecond = 0;
            cout << "Warning: ParticleGenerator with invalid spawn rate change type " << genChgType << endl;
            break;
    }
}
