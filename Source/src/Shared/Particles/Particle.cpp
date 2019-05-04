#include "Shared/Particles/Particle.hpp"
#include <cmath>
using namespace std;
using namespace sf;

float Particle::Behavior::getChange(float timeElapsed, float distanceTraveled) {
    float x = 0;
    if (changeType==Particle::DistanceTraveledChange)
        x = distanceTraveled;
    else if (changeType==Particle::TimeAliveChange)
        x = timeElapsed;
    else if (changeType==Particle::NoChange)
        return 0;

    switch (scaleType) {
        case Constant:
            return multiplier;
        case Linear:
            return multiplier * x;
        case Exponential:
            return multiplier * x*x;
        case Decay:
            return multiplier / x;
        case ExponentialDecay:
            return multiplier / (x*x);
        default:
            return 0;
    }
}

Particle::Particle(const ParticleGraphics& gfx, Vector2f pos, float dir, float vel, float opac, float ctime) {
    graphics = gfx;
    spawnPosition = position = pos;
    velocity = spawnVel = vel;
    direction = spawnDir = dir;
    opacity = spawnOpacity = opac;
    spawnTime = ctime;

    lifetimeType = TimeAliveLifetime;
    lifetimeValue = 3;
}

void Particle::setRotationBehavior(Particle::Behavior behave) {
    rotationBehavior = behave;
}

void Particle::setVelocityBehavior(Particle::Behavior behave) {
    velocityBehavior = behave;
}

void Particle::setOpacityBehavior(Particle::Behavior behave) {
    opacityBehavior = behave;
}

void Particle::setLifetime(Particle::LifetimeType type, float value) {
    lifetimeType = type;
    lifetimeValue = value;
}

float Particle::getSquaredDistance() {
    float dx = spawnPosition.x - position.x;
    float dy = spawnPosition.y - position.y;
    return dx*dx + dy*dy;
}

bool Particle::isFinished(float ctime) {
    switch (lifetimeType) {
        case GeneratorLifetime:
            return false;

        case TimeAliveLifetime:
            return (ctime-spawnTime) >= lifetimeValue;

        case DistanceTraveledLifetime:
            return getSquaredDistance() >= lifetimeValue*lifetimeValue;

        case AnimationDoneLifetime:
            return graphics.finished();

        default:
            cout << "Warning: Particle has invalid Lifetime\n";
            return true;
    }
}

bool Particle::blocksGeneratorDestruction() {
    return lifetimeType != GeneratorLifetime;
}

void Particle::update(float ctime) {
    float timeElapsed = ctime - spawnTime;
    float distanceTraveled = sqrt(getSquaredDistance());
    float dt = ctime - spawnTime;

    opacity = spawnOpacity + opacityBehavior.getChange(timeElapsed, distanceTraveled);
    velocity = spawnVel + velocityBehavior.getChange(timeElapsed, distanceTraveled);
    direction = spawnDir + rotationBehavior.getChange(timeElapsed, distanceTraveled);

    float dirRads = direction * 3.1415926/180;
    position.x += velocity*cos(dirRads) * dt;
    position.y += velocity*sin(dirRads) * dt;
}

void Particle::render(RenderTarget& target, Vector2f genPos) {
    graphics.render(target, genPos+position, direction, opacity);
}
