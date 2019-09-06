#include "Shared/Entities/EntityBehavior.hpp"
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

void EntityBehavior::notifyAttacked(Entity::Ptr attacker, CombatAttack::ConstPtr attack) {
    attackers.push_back(Entity::WeakPtr(attacker));
}

void EntityBehavior::notifyInteracted(Entity::Ptr user) {
    if (!interactor.expired()) {
        cout << "Warning: " << owner->getIdString() << " interacted with by " << user->getIdString()
             << " but has interactor " << interactor.lock()->getIdString() << " active still\n";
    }
    else
        interactor = user;
}

void EntityBehavior::notifyCombatNearby(Entity::List combatants) {
    cout << "Combat detected near " << owner->getIdString() << endl;
}

void EntityBehavior::update() {
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
    else
        state = Default;
}

void EntityBehavior::doFlee() {
    Entity::List enemies = getAttackers();
    if (enemies.size()>0) {
        //TODO - find best runaway path and run that way
    }
    else
        state = Default;
}

void EntityBehavior::doInteract() {
    Entity::Ptr user = getInteractor();
    if (user) {
        //TODO - face them. Walk next to them?
    }
    else
        state = Default;
}
