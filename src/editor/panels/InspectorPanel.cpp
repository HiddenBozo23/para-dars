#include "para-dars/editor/ImGuiCommitInput.h"
#include "para-dars/editor/panels/InspectorPanel.h"
#include "para-dars/ecs/components/NameComponent.h"

InspectorPanel::InspectorPanel(Editor* editor, Scene* scene) 
    : Panel(editor), scene(scene) {
    RegisterDraw<NameComponent>("Name", [](NameComponent& nc) {
        InputTextCommit("name", nc.name);
    });
    RegisterDraw<TransformComponent>("Transform", [](TransformComponent& comp) {
        ImGui::PushItemWidth(ImGui::GetFontSize() * 3);

        ImGui::Text("Position");
        ImGui::SameLine();
        ImGui::InputFloat("X##pos", &comp.position.x);
        ImGui::SameLine();
        ImGui::InputFloat("Y##pos", &comp.position.y);
        ImGui::SameLine();
        ImGui::InputFloat("Z##pos", &comp.position.z);

        glm::vec3 degrees = glm::degrees(comp.rotation);
        ImGui::Text("Rotation");
        ImGui::SameLine();
        if (ImGui::InputFloat("X##rot", &degrees.x)) comp.rotation.x = glm::radians(degrees.x);
        ImGui::SameLine();
        if (ImGui::InputFloat("Y##rot", &degrees.y)) comp.rotation.y = glm::radians(degrees.y);
        ImGui::SameLine();
        if (ImGui::InputFloat("Z##rot", &degrees.z)) comp.rotation.z = glm::radians(degrees.z);

        ImGui::Text("Scale   ");
        ImGui::SameLine();
        ImGui::InputFloat("X##scale", &comp.scale.x);
        ImGui::SameLine();
        ImGui::InputFloat("Y##scale", &comp.scale.y);
        ImGui::SameLine();
        ImGui::InputFloat("Z##scale", &comp.scale.z);

        ImGui::PopItemWidth();
    });
}

void InspectorPanel::OnImGuiRender() {
    ImGui::Begin("Inspector");
    EntityID selected = GetEditor()->GetSelectedEntity();
    if (selected==INVALID_ENTITY) {
        ImGui::End();
        return;
    };

    ImGui::BeginChild("InspectorScroll", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);
    std::string entityLabel = std::to_string(selected) + "; ";
    ImGui::TextUnformatted(entityLabel.c_str());

    for (auto& [type, drawData] : componentDrawers) {
        if (drawData.checkFunc(selected)) {
            bool open = true;
            if (ImGui::CollapsingHeader(drawData.name.c_str(), &open, ImGuiTreeNodeFlags_DefaultOpen)) {
                if (!open) {
                    drawData.removeFunc(selected);
                } else {
                    drawData.drawFunc(selected);
                }
            }
        }
    }

    ImGui::EndChild();
    ImGui::End();
}
