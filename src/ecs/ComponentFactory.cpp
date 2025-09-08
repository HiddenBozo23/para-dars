#include "para-dars/ecs/ComponentFactory.h"
#include "para-dars/core/LogManager.h"

std::shared_ptr<Serialisable> ComponentFactory::Create(const std::string& typeName) {
    if (typeName == "TransformComponent") return std::make_shared<TransformComponent>();
    if (typeName == "CameraComponent") return std::make_shared<CameraComponent>();
    if (typeName == "ModelComponent") return std::make_shared<ModelComponent>();
    if (typeName == "HierarchyComponent") return std::make_shared<HierarchyComponent>();
    if (typeName == "NameComponent") return std::make_shared<NameComponent>();
    if (typeName == "RenderComponent") return std::make_shared<RenderComponent>();

    LogManager::Log(LogType::Error, "ERROR: COMPONENT FACTORY DOES NOT RECOGNISE TYPE: " + typeName);
    return nullptr;
}