#include "Shared/Entities/EntityBehavior.hpp"
using namespace std;

EntityBehavior::EntityBehavior(Entity::Ptr ent) {
    owner = ent;
}

Entity::Ptr EntityBehavior::getInteractor() {
    if (!interactor.expired())
        return interactor.lock();
    return Entity::Ptr(nullptr);
}

list<Entity::Ptr> EntityBehavior::getAttackers() {
    list<Entity::Ptr> ret;
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

void EntityBehavior::notifyAttacked(Entity::Ptr attacker) {
    attackers.push_back(Entity::WeakPtr(attacker));
}

void EntityBehavior::notifyInteracted(Entity::Ptr user) {
    if (!interactor.expired()) {
        cout << "Warning: " << owner.lock()->getIdString() << " interacted with by " << user->getIdString() << " but has interactor " << interactor.lock()->getIdString() << " active still\n";
    }
    interactor = user;
}

void EntityBehavior::notifyCombatNearby(Entity::List combatants) {
    cout << "Combat detected near " << owner.lock()->getIdString() << endl;
}
