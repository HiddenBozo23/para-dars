#pragma once

#include "para-dars/editor/Panel.h"
#include "para-dars/ecs/systems/HierarchySystem.h"

class HierarchyPanel : public Panel {
    public:
        HierarchyPanel(Editor* editor, HierarchySystem* hierarchySystem);

        void OnImGuiRender() override;

    private:
        HierarchySystem* hierarchySystem;

        void DrawEntity(EntityID& entity, Scene* scene, std::vector<EntityID>& entities);
};
