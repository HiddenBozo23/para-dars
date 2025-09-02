#include "para-dars/ecs/Scene.h"

EntityID Scene::CreateEntity() {
    EntityID id = EntityManager::CreateEntity();
    entities.push_back(id);
    return id;
}

void Scene::DestroyEntity(EntityID id) {
    if (std::find(entities.begin(), entities.end(), id) == entities.end()) return;

    for(auto& [type, entityMap] : componentMap) entityMap.erase(id);
    EntityManager::DestroyEntity(id);
}

void Scene::AddComponent(std::type_index type, EntityID id, std::shared_ptr<void> component) {
    componentMap[type][id] = std::move(component);
    NotifyComponentChanged(id);
}

void Scene::RemoveComponent(std::type_index type, EntityID id) {
    for (auto system : systems) system->OnComponentAboutToBeRemoved(type, id);

    auto typeIt = componentMap.find(type);
    if (typeIt != componentMap.end()) {
        typeIt->second.erase(id);
        NotifyComponentChanged(id);
    }
}

bool Scene::HasComponent(std::type_index type, EntityID id) {
    auto typeIt = componentMap.find(type);
    if (typeIt == componentMap.end()) return false;
    return typeIt->second.find(id) != typeIt->second.end();
}

void* Scene::GetComponent(std::type_index type, EntityID id) {
    auto typeIt = componentMap.find(type);
    if (typeIt == componentMap.end()) return nullptr;
    if (typeIt->second.find(id) == typeIt->second.end()) return nullptr;
    return typeIt->second.find(id)->second.get();
}

void Scene::RegisterSystem(System* system) {
    systems.push_back(system);
}

void Scene::NotifyComponentChanged(EntityID id) {
    for (auto system : systems) {
        if (system) system->OnComponentChanged(id);
    }
}