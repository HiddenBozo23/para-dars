#pragma once

#include <memory>

#include "para-dars/ecs/EntityManager.h"
#include "para-dars/editor/Panel.h"
#include "para-dars/ecs/components/TransformComponent.h"
#include "para-dars/ecs/components/CameraComponent.h"
#include "para-dars/ecs/Scene.h"
#include "para-dars/ecs/systems/HierarchySystem.h"
#include "para-dars/ecs/systems/RenderSystem.h"

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

class Editor {
    public:
        Editor();
        void InitPanels();

        void OnImGuiRender();
        void SetSelectedEntity(EntityID entity);
        EntityID GetSelectedEntity();

        void RegisterPanel(std::unique_ptr<Panel> panel);

    private:
        void RenderMainMenuBar();

        std::unique_ptr<HierarchySystem> hierarchySystem;
        std::unique_ptr<RenderSystem> renderSystem;

        std::unique_ptr<Scene> currentScene;
        EntityID selectedEntity = INVALID_ENTITY;
        std::vector<std::unique_ptr<Panel>> panels;
};
