#include "para-dars/ecs/systems/HierarchySystem.h"
#include "para-dars/ecs/components/HierarchyComponent.h"

HierarchySystem::HierarchySystem(Scene* scene)
    : System(scene) {
    requiredComponents = {
        typeid(HierarchyComponent)
    };
}

void HierarchySystem::OnEntityAboutToBeRemoved(EntityID id) {
    auto hc = GetScene()->GetComponent<HierarchyComponent>(id); 
    EntityID newParent = hc->parent;
    // reparent all children
    for (EntityID child : hc->children) {
        SetParent(child, newParent);
    }
    // remove from parent's child list
    if (newParent != INVALID_ENTITY) {
        auto parentHC = GetScene()->GetComponent<HierarchyComponent>(newParent);
        auto& siblings = parentHC->children; // & because we want changes to affect source
        siblings.erase(std::remove(siblings.begin(), siblings.end(), id), siblings.end());
    }
}

void HierarchySystem::SetParent(EntityID childID, EntityID parentID) {
    auto childHC = GetScene()->GetComponent<HierarchyComponent>(childID);
    if (childHC->parent != INVALID_ENTITY) {
        auto oldParentHC = GetScene()->GetComponent<HierarchyComponent>(childHC->parent);
        auto& siblings=oldParentHC->children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), childID), siblings.end());
    }
    childHC->parent = parentID;
    if(parentID != INVALID_ENTITY) {
        auto newParentHC = GetScene()->GetComponent<HierarchyComponent>(parentID);
        newParentHC->children.push_back(childID);
    }
}