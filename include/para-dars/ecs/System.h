#pragma once

#include <typeindex>
#include <memory>

#include "para-dars/ecs/EntityManager.h"

class Scene;  // forward declaration to break cyclic dependancy

class System {
    public:
        explicit System(Scene* scene);
        virtual ~System() = default;

        virtual void Update(float deltaTime) = 0;

        virtual void OnEntityAdded(EntityID id) {}
        virtual void OnEntityAboutToBeRemoved(EntityID id) {}
        virtual void OnComponentChanged(EntityID id);
        virtual void OnComponentAboutToBeRemoved(std::type_index type, EntityID id);
        virtual Scene* GetScene();
        virtual std::vector<EntityID> GetTrackedEntities();

    protected:
        std::vector<std::type_index> requiredComponents;
        std::vector<EntityID> trackedEntities;
        
    private:
        Scene* scene;
        std::unique_ptr<Scene> backupScene;
};

#include "para-dars/ecs/Scene.h"  // import properly