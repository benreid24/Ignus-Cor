#include "Shared/Entities/Behaviors/EntityBehavior.hpp"
#include "Shared/Properties.hpp"
using namespace std;

EntityBehavior::EntityBehavior(Entity* ent) {
    owner = ent;
    state = Default;
    combatTendency = Normal;
    startPosition = ent->getPosition();
}

Entity::Ptr EntityBehavior::getInteractor() {
    if (!interactor.expired())
        return interactor.lock();
    return Entity::Ptr(nullptr);
}

Entity::List EntityBehavior::getAttackers() {
    Entity::List ret;
    for (list<Entity::WeakPtr>::iterator i = attackers.begin(); i!=attackers.end(); ++i) {
        if (!i->expired())
            ret.push_back(i->lock());
        else {
            auto e = i--;
            attackers.erase(++e);
        }
    }
    return ret;
}

void EntityBehavior::notifyAttacked(Entity::Ptr attacker, CombatAttack::Ref attack) {
    attackers.push_back(Entity::WeakPtr(attacker));
    p_notifyAttacked(attacker, attack);
}

void EntityBehavior::notifyInteracted(Entity::Ptr user) {
    if (!interactor.expired()) {
        if (interactor.lock().get() != user.get())
            cout << "Warning: " << owner->getIdString() << " interacted with by " << user->getIdString()
                 << " but has interactor " << interactor.lock()->getIdString() << " active still\n";
    }
    else {
        interactor = user;
        p_notifyInteracted(user);
    }
}

void EntityBehavior::terminateInteraction() {
    if (state == Interacting)
        state = state.lastState;
    interactor.reset();
}

void EntityBehavior::notifyCombatNearby(Entity::List combatants) {
    cout << "Combat detected near " << owner->getIdString() << endl;
    p_notifyCombatNearby(combatants);
}

void EntityBehavior::update() {
    if (!interactor.expired()) {
        Entity::Ptr user = interactor.lock();
        sf::Vector2f dist = owner->getPosition().coords - user->getPosition().coords;
        if (abs(dist.x) >= Properties::ConversationTerminationDistance ||
            abs(dist.y) >= Properties::ConversationTerminationDistance ||
            user->getPosition().mapName != owner->getPosition().mapName) {
                terminateInteraction();
            }
    }

    p_update(); //derived class sets state. Can set to the below states to get shared behavior

    switch (state) {
        case Fleeing:
            doFlee();
            break;

        case Fighting:
        case Aiding:
            doFight();
            break;

        case Interacting:
            doInteract();
            break;

        default:
            break;
    }
}

void EntityBehavior::doFight() {
    Entity::List enemies = getAttackers();
    if (enemies.size()>0) {
        //TODO - figure out how to fight
    }
}

void EntityBehavior::doFlee() {
    Entity::List enemies = getAttackers();
    if (enemies.size()>0) {
        //TODO - find best runaway path and run that way
    }
}

void EntityBehavior::doInteract() {
    Entity::Ptr user = getInteractor();
    if (user) {
        EntityPosition::Direction dir;
        sf::Vector2f myPos = owner->getPosition().coords;
        sf::Vector2f iPos = user->getPosition().coords;
        if (abs(myPos.x-iPos.x) > abs(myPos.y-iPos.y)) {
            if (myPos.x > iPos.x)
                dir = EntityPosition::Left;
            else
                dir = EntityPosition::Right;
        }
        else if (myPos.y > iPos.y)
            dir = EntityPosition::Up;
        else
            dir = EntityPosition::Down;
        if (owner->getPosition().dir != dir)
            owner->move(dir, false);
    }
    else
        terminateInteraction();
}
