#pragma once

#include "para-dars/ecs/components/TransformComponent.h"
#include "para-dars/ecs/components/CameraComponent.h"
#include "para-dars/ecs/components/ModelComponent.h"
#include "para-dars/ecs/components/HierarchyComponent.h"
#include "para-dars/ecs/components/NameComponent.h"
#include "para-dars/ecs/components/RenderComponent.h"

class ComponentFactory {
    public:
        static std::shared_ptr<Serialisable> Create(const std::string& typeName);
};