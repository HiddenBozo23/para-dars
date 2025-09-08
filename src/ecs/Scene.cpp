#include "para-dars/ecs/Scene.h"
#include "para-dars/ecs/Serialisable.h"
#include "para-dars/core/LogManager.h"
#include "para-dars/ecs/ComponentFactory.h"

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

std::string Scene::Serialise() {
    nlohmann::json sceneJ;
    sceneJ["entities"] = nlohmann::json::array();

    for (EntityID id : entities) {
        nlohmann::json entityJ;
        entityJ["id"] = id;
        entityJ["components"] = nlohmann::json::array();

        for (auto& [type, entityMap] : componentMap) {
            auto it = entityMap.find(id);
            if (it != entityMap.end()) {
                Serialisable* serial = static_cast<Serialisable*>(it->second.get());
                if (serial) {
                    nlohmann::json compJ = nlohmann::json::parse(serial->Serialise());
                    compJ["type"] = type.name();
                    entityJ["components"].push_back(compJ);
                }
                else LogManager::Log(LogType::Error, "ERROR: FAILED TO SERIALISE COMPONENT: ID: " + std::to_string(id) + " COMPONENT: " + type.name());
            }
        }

        sceneJ["entities"].push_back(entityJ);
    }

    return sceneJ.dump(4);
}

void Scene::Deserialise(const std::string& data) {
    nlohmann::json sceneJ = nlohmann::json::parse(data);

    entities.clear();
    componentMap.clear();
    for (auto system : systems) 
        if (system) system->ClearTrackedEntities();

    for (auto& entityJ : sceneJ["entities"]) {

        EntityID id  = entityJ["id"];
        entities.push_back(id);

        for (auto& compJ : entityJ["components"]) {
            std::string typeName = compJ["type"];
            auto comp = ComponentFactory::Create(typeName);
            if (comp) {
                comp->Deserialise(compJ.dump());
                AddComponent(std::type_index(typeid(*comp)), id, comp);
            }
        }
    }
}
