#pragma once

#include "para-dars/ecs/System.h"

class HierarchySystem : public System {
    public:
        explicit HierarchySystem(Scene* scene);
        
        void Update(float deltaTime) override {}
        void OnEntityAboutToBeRemoved(EntityID id) override;

        void SetParent(EntityID childID, EntityID parentID);
};
