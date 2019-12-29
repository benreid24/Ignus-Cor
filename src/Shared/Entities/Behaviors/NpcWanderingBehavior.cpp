#include "Shared/Entities/Behaviors/NpcWanderingBehavior.hpp"
#include "Shared/Util/Util.hpp"
#include "Shared/Util/Timer.hpp"
using namespace std;

NpcWanderingBehavior::NpcWanderingBehavior(Entity* ent, int rad) : EntityBehavior(ent) {
    radius = rad;
    sqrdRad = rad*rad;
    timeOfNextStep = 0;
}

void NpcWanderingBehavior::p_update() {
    switch (state) {
        case Default:
            if (Timer::get().timeElapsedMilliseconds()-lastStepTime<=timeOfNextStep) {
                if (!owner->move(movementDirection)) {
                    movementDirection = getMovementDirection();
                }
            }
            else {
                int chanceToStop = randomInt(0,100);
                if (chanceToStop<=30)
                    state = Standing;
                else {
                    movementDirection = getMovementDirection();
                    timeOfNextStep = 500 + randomInt(0, 1000);
                }
                lastStepTime = Timer::get().timeElapsedMilliseconds();
            }
            break;

        case Standing:
            if (Timer::get().timeElapsedMilliseconds()-lastStepTime>2000) {
                int chanceToStart = randomInt(0,100);
                if (chanceToStart<=10) {
                    state = Default;
                    movementDirection = getMovementDirection();
                    timeOfNextStep = 500 + randomInt(0, 1000);
                    lastStepTime = Timer::get().timeElapsedMilliseconds();
                }
            }
            break;

        default:
            break;
    }
}

void NpcWanderingBehavior::p_notifyInteracted(Entity::Ptr interactor) {
    state = Interacting;
}

EntityPosition::Direction NpcWanderingBehavior::getMovementDirection() {
    if (radius==0)
        return EntityPosition::Direction(randomInt(0,3));

    if (randomInt(0,100)<=50)
    {
        int dy = owner->getPosition().coords.y - startPosition.coords.y;
        double sqrdDy = dy*dy;
        bool goBack = (sqrdDy/sqrdRad)*1000 <= randomInt(0, 1000);
        if (goBack) {
            if (dy>=0)
                return EntityPosition::Down;
            else
                return EntityPosition::Up;
        }
        else {
            if (dy>=0)
                return EntityPosition::Up;
            else
                return EntityPosition::Down;
        }
    }
    else
    {
        int dx = owner->getPosition().coords.x - startPosition.coords.x;
        double sqrdDx = dx*dx;
        bool goBack = (sqrdDx/sqrdRad)*1000 <= randomInt(0, 1000);
        if (goBack) {
            if (dx>=0)
                return EntityPosition::Right;
            else
                return EntityPosition::Left;
        }
        else {
            if (dx>=0)
                return EntityPosition::Left;
            else
                return EntityPosition::Right;
        }
    }
}

int NpcWanderingBehavior::magnitudeSquared(sf::Vector2i v)
{
    return v.x*v.x + v.y*v.y;
}
