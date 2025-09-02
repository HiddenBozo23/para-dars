#include "para-dars/ecs/EntityManager.h"

EntityID EntityManager::nextID = 1;
std::vector<bool> EntityManager::alive;
std::vector<EntityID> EntityManager::freeIDs;

EntityID EntityManager::CreateEntity() {
    EntityID id;
    if(!freeIDs.empty()) {
        id = freeIDs.back();
        freeIDs.pop_back();
    } else {
        id = nextID++;
        if (alive.size() <= id) alive.resize(id + 1, false);
    }
    
    alive[id] = true;
    return id;
}

void EntityManager::DestroyEntity(EntityID id) {
    if (id == INVALID_ENTITY || id >= alive.size() || !alive[id]) return;

    alive[id] = false;
    freeIDs.push_back(id);
}

bool EntityManager::isValid(EntityID id) {
    return id != INVALID_ENTITY && id < alive.size() && alive[id];
}