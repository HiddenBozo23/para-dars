#include <imgui.h>
#include <imgui_internal.h>

#include "para-dars/editor/Editor.h"
#include "para-dars/core/LogManager.h"
#include "para-dars/core/AssetManager.h"
#include "para-dars/editor/panels/ViewportPanel.h"
#include "para-dars/editor/panels/HierarchyPanel.h"
#include "para-dars/editor/panels/InspectorPanel.h"
#include "para-dars/editor/panels/LogPanel.h"

#include "para-dars/ecs/components/HierarchyComponent.h"
#include "para-dars/ecs/components/ModelComponent.h"
#include "para-dars/ecs/components/RenderComponent.h"

unsigned int SCR_WIDTH = 1500;
unsigned int SCR_HEIGHT = 800;

Editor::Editor() {
    currentScene = std::make_unique<Scene>();
    hierarchySystem = std::make_unique<HierarchySystem>(currentScene.get());
    renderSystem = std::make_unique<RenderSystem>(currentScene.get(), SCR_WIDTH, SCR_HEIGHT);
    currentScene->RegisterSystem(hierarchySystem.get());
    currentScene->RegisterSystem(renderSystem.get());

    EntityID e = currentScene->CreateEntity();
    currentScene->AddComponent<HierarchyComponent>(e, std::make_shared<HierarchyComponent>());
    currentScene->AddComponent<TransformComponent>(e, std::make_shared<TransformComponent>());
    currentScene->AddComponent<ModelComponent>(e, std::make_shared<ModelComponent>(AssetManager::LoadModel("assets/models/crate/Crate1.obj")));
    currentScene->AddComponent<RenderComponent>(e, std::make_shared<RenderComponent>());
}

void Editor::InitPanels() {
    RegisterPanel(std::make_unique<LogPanel>(this));
    RegisterPanel(std::make_unique<InspectorPanel>(this, currentScene.get()));
    RegisterPanel(std::make_unique<HierarchyPanel>(this, hierarchySystem.get()));
    RegisterPanel(std::make_unique<ViewportPanel>(this, renderSystem.get()));
}

void Editor::SetScene(Scene scene) {

}

void Editor::OnImGuiRender() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

    static bool first_time = true;
    if (first_time) {
        first_time = false;

        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        ImGuiID dock_main_id = dockspace_id;
        ImGuiID dock_left, dock_right, dock_bottom, dock_center;

        dock_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left,   0.15f, nullptr, &dock_main_id);
        dock_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right,  0.2f, nullptr, &dock_main_id);
        dock_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down,   0.2f, nullptr, &dock_main_id);
        dock_center = dock_main_id;

        ImGui::DockBuilderDockWindow("Hierarchy", dock_left);
        ImGui::DockBuilderDockWindow("Inspector", dock_right);
        ImGui::DockBuilderDockWindow("Log", dock_bottom);
        ImGui::DockBuilderDockWindow("Viewport", dock_center);

        ImGui::DockBuilderFinish(dockspace_id);
    }

    RenderMainMenuBar();

    for (const auto& panel : panels) 
        panel->OnImGuiRender();
    
    ImGui::End();
}

void Editor::SetSelectedEntity(EntityID entity) {
    if (selectedEntity != entity)
        selectedEntity = entity;
}

EntityID Editor::GetSelectedEntity() {
    return selectedEntity;
}

void Editor::RegisterPanel(std::unique_ptr<Panel> panel) {
    panels.push_back(std::move(panel));
}

void Editor::RenderMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("New")) {
                // Handle New
            }
            if (ImGui::MenuItem("Open")) {
                // Handle Open
            }
            if (ImGui::MenuItem("Save")) {
                // Handle Save
            }
            if (ImGui::MenuItem("Exit")) {
                // Handle Exit
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Entity")) {
            if (ImGui::MenuItem("Add")) {
                // Handle New
            }
            if (ImGui::MenuItem("Remove")) {
                if (selectedEntity == INVALID_ENTITY)
                    LogManager::Log(LogType::Error, "ERROR: NO ENTITY SELECTED, CANNOT REMOVE");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}