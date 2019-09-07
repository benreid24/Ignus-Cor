#include "Shared/Particles/ParticleGenerator.hpp"
#include "Shared/Util/Util.hpp"
#include "Shared/Util/Timer.hpp"
using namespace std;
using namespace sf;

float ParticleGenerator::ValueWindow::getValue() {
    float range = maximum - minimum;
    float r = randomInt(0, 100000);
    return minimum + r/100000*range;
}

ParticleGenerator::ParticleGenerator(ParticleGenerator::ValueWindow radius, ParticleGenerator::ValueWindow angle,
                                     ParticleGenerator::ValueWindow speed, ParticleGenerator::ValueWindow dir)
: spawnRadius(radius), spawnAngle(angle), spawnVelocity(speed), spawnDirection(dir), spawnOpacity(255,255) {
    lifetimeType = UntilDestroyedLifetime;
    particlesPerSecond = 0;
    totalCreated = 0;
    startRate = 0;
    lastSpawnTime = startTime = zeroTime = Timer::get().timeElapsedSeconds();
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
    particlesPerSecond = startRate = rate;
    genChgType = rateChgType;
    genChgMultiplier = rateChgMultiplier;
}

void ParticleGenerator::stop(float stopTime) {
    if (stopTime==-1)
        stopTime = lifetimeValue;
    genChgType = Linear;
    startRate = particlesPerSecond;
    if (stopTime!=0)
        genChgMultiplier = -particlesPerSecond/stopTime;
    else {
        particlesPerSecond = 0;
        genChgMultiplier = 0;
        startRate = 0;
    }
    zeroTime = Timer::get().timeElapsedSeconds();
}

bool ParticleGenerator::finished() {
    if (lifetimeType==TimeExistedLifetime)
        return Timer::get().timeElapsedSeconds()-startTime >= lifetimeValue;
    else if (lifetimeType==ParticlesGeneratedLifetime)
        return totalCreated >= lifetimeValue;
    else if (lifetimeType==UntilDestroyedLifetime) {
        if (totalCreated==0)
            return false; //make at least 1

        for (auto i = particles.begin(); i!=particles.end(); ++i) {
            if (!(*i)->finished(Timer::get().timeElapsedSeconds()) && (*i)->blocksGeneratorDestruction())
                return false;
        }
        return true;
    }
    else {
        cout << "Warning: Invalid ParticleGenerator lifetime type\n";
        return true;
    }
}

void ParticleGenerator::update() {
    updateSpawnRate();
    spawnParticles();
    for (auto i = particles.begin(); i!=particles.end(); ++i) {
        (*i)->update(Timer::get().timeElapsedSeconds());
        if ((*i)->finished(Timer::get().timeElapsedSeconds())) {
            auto j = i--;
            delete *j;
            particles.erase(j);
        }
    }
}

void ParticleGenerator::render(RenderTarget& target, Vector2f pos, Vector2f camPos) {
    for (auto i = particles.begin(); i!=particles.end(); ++i) {
        (*i)->render(target, pos, camPos);
    }
}

void ParticleGenerator::spawnParticles() {
    float dt = Timer::get().timeElapsedSeconds() - lastSpawnTime;
    int spawnCount = particlesPerSecond * dt;
    ValueWindow lifetimeWindow(particleLifetimeValue-particleLifetimeFuzziness, particleLifetimeValue+particleLifetimeFuzziness);

    if (spawnCount>0) {
        lastSpawnTime = Timer::get().timeElapsedSeconds();
    }

    totalCreated += spawnCount;
    for (int i = 0; i<spawnCount; ++i) {
        float angle = spawnAngle.getValue() * 3.1415926/180;
        float radius = spawnRadius.getValue();
        float x = radius*cos(angle), y = radius*sin(angle);
        Particle* p = new Particle(defaultParticleGraphics, Vector2f(x,y), spawnDirection.getValue(),
                                   spawnVelocity.getValue(), spawnOpacity.getValue(), Timer::get().timeElapsedSeconds());
        p->setRotationBehavior(rotationBehavior);
        p->setVelocityBehavior(velocityBehavior);
        p->setOpacityBehavior(opacityBehavior);
        p->setLifetime(particleLifetimeType, lifetimeWindow.getValue());
        particles.push_back(p);
    }
}

void ParticleGenerator::updateSpawnRate() {
    float dt = Timer::get().timeElapsedSeconds() - zeroTime;
    switch (genChgType) {
        case Constant:
            particlesPerSecond = genChgMultiplier * dt;
            break;

        case Linear:
            particlesPerSecond = startRate + genChgMultiplier * dt;
            break;

        case Exponential:
            particlesPerSecond = startRate + genChgMultiplier * dt*dt;
            break;

        case Decay:
            particlesPerSecond = startRate + genChgMultiplier / dt;

        case ExponentialDecay:
            particlesPerSecond = startRate + genChgMultiplier / (dt*dt);
            break;

        default:
            particlesPerSecond = 0;
            cout << "Warning: ParticleGenerator with invalid spawn rate change type " << genChgType << endl;
            break;
    }
}
