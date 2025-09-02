#include <imgui.h>

#include "para-dars/editor/panels/HierarchyPanel.h"
#include "para-dars/ecs/components/HierarchyComponent.h"
#include "para-dars/ecs/components/NameComponent.h"

HierarchyPanel::HierarchyPanel(Editor* editor, HierarchySystem* hierarchySystem)
    : Panel(editor), hierarchySystem(hierarchySystem) {}

void HierarchyPanel::OnImGuiRender() {
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
    // unparent drag drop function
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 1.0f, 1.0f), "Drop Here to Unparent");
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG")) {
            EntityID droppedEntity = *(const EntityID*)payload->Data;
            hierarchySystem->SetParent(droppedEntity, INVALID_ENTITY);
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::Separator();

    Scene* scene = hierarchySystem->GetScene();
    std::vector<EntityID> entities = hierarchySystem->GetTrackedEntities();
    for (EntityID entity : entities) {
        if (scene->GetComponent<HierarchyComponent>(entity)->parent == INVALID_ENTITY) 
            DrawEntity(entity, scene, entities);
    }

    ImGui::End();
}

void HierarchyPanel::DrawEntity(EntityID& entity, Scene* scene, std::vector<EntityID>& entities) {
    std::string label = "Entity " + std::to_string(entity);
    NameComponent* nameComp = scene->GetComponent<NameComponent>(entity);
    if (nameComp) label = nameComp->name;

    HierarchyComponent* hc = scene->GetComponent<HierarchyComponent>(entity);
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
    if (hc->children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;
    if (GetEditor()->GetSelectedEntity() == entity) flags |= ImGuiTreeNodeFlags_Selected;

    bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)entity, flags, "%s", label.c_str());

    if (ImGui::IsItemClicked()) GetEditor()->SetSelectedEntity(entity);
    // drag source and drop to parent to target
    if (ImGui::BeginDragDropSource()) {
        EntityID draggedEntity = entity;
        ImGui::SetDragDropPayload("ENTITY_DRAG", & draggedEntity, sizeof(EntityID));
        ImGui::Text("Move %s", label.c_str());
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG")) {
            EntityID droppedEntity = *(const EntityID*)payload->Data;
            if (droppedEntity != entity) hierarchySystem->SetParent(droppedEntity, entity);
        }
        ImGui::EndDragDropTarget();
    }

    if (nodeOpen) {
        for (EntityID child : hc->children) DrawEntity(child, scene, entities);
        ImGui::TreePop();
    }
}