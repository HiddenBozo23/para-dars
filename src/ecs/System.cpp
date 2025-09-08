#include "para-dars/ecs/System.h"
#include "para-dars/core/LogManager.h"

System::System(Scene* scene) 
    : scene(scene) {}

void System::OnComponentChanged(EntityID id) {
    // does the new id meet system requirements?
    bool matches = true;
    for (const auto& type : requiredComponents) {
        if (!GetScene()->HasComponent(type, id)) {
            matches = false;
            break;
        }
    }
    auto it = std::find(trackedEntities.begin(), trackedEntities.end(), id);
    if (matches && it == trackedEntities.end()) {
        trackedEntities.push_back(id);
        OnEntityAdded(id);
    } else if (!matches && it != trackedEntities.end())
        trackedEntities.erase(it);
}

void System::OnComponentAboutToBeRemoved(std::type_index type, EntityID id) {
    if (std::find(requiredComponents.begin(), requiredComponents.end(), type) != requiredComponents.end())
        OnEntityAboutToBeRemoved(id);
}

Scene* System::GetScene() {
    if (scene) return scene;

    LogManager::Log(LogType::Error, "ERROR: INVALID SCENE POINTER IN SYSTEM, USING DEFAULT EMPTY SCENE");
    return nullptr;
}

std::vector<EntityID> System::GetTrackedEntities() {
    return trackedEntities;
}

void System::ClearTrackedEntities() {
    trackedEntities.clear();
}