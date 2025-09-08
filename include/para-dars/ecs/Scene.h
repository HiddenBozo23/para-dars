#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "para-dars/ecs/EntityManager.h"
#include "para-dars/ecs/System.h"
#include "para-dars/ecs/Serialisable.h"

class Scene : Serialisable {
    public:
        EntityID CreateEntity();
        void DestroyEntity(EntityID id);

        void AddComponent(std::type_index type, EntityID id, std::shared_ptr<void> component);
        void RemoveComponent(std::type_index type, EntityID id);
        bool HasComponent(std::type_index type, EntityID id);
        void* GetComponent(std::type_index type, EntityID id);

        template<typename T>
        void AddComponent(EntityID id, std::shared_ptr<T> component);
        template<typename T>
        void RemoveComponent(EntityID id);
        template<typename T>
        bool HasComponent(EntityID id);
        template<typename T>
        T* GetComponent(EntityID id);

        void RegisterSystem(System* system);
        void NotifyComponentChanged(EntityID id);

        std::string Serialise() override;
        void Deserialise(const std::string& data);
    
    private:
        std::vector<EntityID> entities;
        std::unordered_map<std::type_index, std::unordered_map<EntityID, std::shared_ptr<void>>> componentMap;
        std::vector<System*> systems;
};

// templates must be defined in header or inl file
template<typename T>
void Scene::AddComponent(EntityID id, std::shared_ptr<T> component) {
    AddComponent(typeid(T), id, std::move(component));
}

template<typename T>
void Scene::RemoveComponent(EntityID id) {
    RemoveComponent(typeid(T), id);
}

template<typename T>
bool Scene::HasComponent(EntityID id) {
    return HasComponent(typeid(T), id);
}

template<typename T>
T* Scene::GetComponent(EntityID id) {
    void* ptr = GetComponent(typeid(T), id);
    return static_cast<T*>(ptr);
}