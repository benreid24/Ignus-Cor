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
    p_notifyAttacked(attacker, attack);
}

void EntityBehavior::notifyInteracted(Entity::Ptr user) {
    if (!interactor.expired()) {
        cout << "Warning: " << owner->getIdString() << " interacted with by " << user->getIdString()
             << " but has interactor " << interactor.lock()->getIdString() << " active still\n";
    }
    else
        interactor = user;
    p_notifyInteracted(user);
}

void EntityBehavior::notifyCombatNearby(Entity::List combatants) {
    cout << "Combat detected near " << owner->getIdString() << endl;
    p_notifyCombatNearby(combatants);
}

void EntityBehavior::update() {
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
        //TODO - face them. Walk next to them?
    }
}
