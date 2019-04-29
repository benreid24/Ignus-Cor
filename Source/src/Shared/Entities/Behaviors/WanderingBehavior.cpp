#include "Shared/Entities/Behaviors/WanderingBehavior.hpp"
#include "Shared/Util/Util.hpp"
using namespace std;

WanderingBehavior::WanderingBehavior(Entity* ent, int rad) : EntityBehavior(ent) {
    radius = rad;
    timeOfNextStep = 0;
}

void WanderingBehavior::update() {
    EntityBehavior::update();

    switch (state) {
        case Default:
            if (timer.getElapsedTime().asMilliseconds()<=timeOfNextStep) {
                if (!owner->move(movementDirection)) {
                    movementDirection = getMovementDirection();
                }
            }
            else {
                int chanceToStop = Random(0,100);
                if (chanceToStop<=30)
                    state = Standing;
                else {
                    movementDirection = getMovementDirection();
                    timeOfNextStep = 1200 + Random(0, 1000);
                }
                timer.restart();
            }
            break;

        case Standing:
            if (timer.getElapsedTime().asMilliseconds()>2000) {
                int chanceToStart = Random(0,100);
                if (chanceToStart<=10) {
                    state = Default;
                    movementDirection = getMovementDirection();
                    timeOfNextStep = 1200 + Random(0, 1000);
                }
            }
            break;

        default:
            break;
    }
}

EntityPosition::Direction WanderingBehavior::getMovementDirection() {
    if (radius==0)
        return EntityPosition::Direction(Random(0,3));

    sf::Vector2i pos = sf::Vector2i(owner->getPosition().coords);
    int xWeight = abs(pos.x-startPosition.coords.x);
    int yWeight = abs(pos.y-startPosition.coords.y);

    bool moveY = (abs(xWeight-yWeight)<=2)?Random(0,1):Random(0,yWeight+xWeight)<yWeight;
    if (moveY)
    {
        int chance = Random(0,1000);
        if (chance<530)
        {
            if (pos.y<startPosition.coords.y)
                return EntityPosition::Direction::Down;
            else
                return EntityPosition::Direction::Up;
        }
        else
        {
            if (pos.y<startPosition.coords.y)
            {
                if (magnitudeSquared(sf::Vector2i(abs(pos.x-startPosition.coords.x),abs(pos.y-startPosition.coords.y)+1))<radius*radius || radius==0)
                    return EntityPosition::Direction::Up;
                else
                    return EntityPosition::Direction::Down;
            }
            else
            {
                if (magnitudeSquared(sf::Vector2i(abs(pos.x-startPosition.coords.x),abs(pos.y-startPosition.coords.y)+1))<radius*radius || radius==0)
                    return EntityPosition::Direction::Down;
                else
                    return EntityPosition::Direction::Up;
            }
        }
    }
    else
    {
        int chance = Random(0,1000);
        if (chance<530)
        {
            if (pos.x<startPosition.coords.x)
                return EntityPosition::Direction::Right;
            else
                return EntityPosition::Direction::Left;
        }
        else
        {
            if (pos.x<startPosition.coords.x)
            {
                if (magnitudeSquared(sf::Vector2i(abs(pos.x-startPosition.coords.x)+1,abs(pos.y-startPosition.coords.y)))<radius*radius || radius==0)
                    return EntityPosition::Direction::Left;
                else
                    return EntityPosition::Direction::Right;
            }
            else
            {
                if (magnitudeSquared(sf::Vector2i(abs(pos.x-startPosition.coords.x)+1,abs(pos.y-startPosition.coords.y)))<radius*radius || radius==0)
                    return EntityPosition::Direction::Right;
                else
                    return EntityPosition::Direction::Left;
            }
        }
    }
}

int WanderingBehavior::magnitudeSquared(sf::Vector2i v)
{
    return v.x*v.x + v.y*v.y;
}
