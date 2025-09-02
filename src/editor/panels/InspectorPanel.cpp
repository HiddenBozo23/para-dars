#include "para-dars/editor/panels/InspectorPanel.h"
#include "para-dars/editor/ImGuiCommitInput.h"
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
        InputFloatCommit("X##pos", comp.position.x);
        ImGui::SameLine();
        InputFloatCommit("Y##pos", comp.position.y);
        ImGui::SameLine();
        InputFloatCommit("Z##pos", comp.position.z);

        glm::vec3 degrees = glm::degrees(comp.rotation);
        ImGui::Text("Rotation");
        ImGui::SameLine();
        if (InputFloatCommit("X##rot", degrees.x)) comp.rotation.x = glm::radians(degrees.x);
        ImGui::SameLine();
        if (InputFloatCommit("Y##rot", degrees.y)) comp.rotation.y = glm::radians(degrees.y);
        ImGui::SameLine();
        if (InputFloatCommit("Z##rot", degrees.z)) comp.rotation.z = glm::radians(degrees.z);

        ImGui::Text("Scale   ");
        ImGui::SameLine();
        InputFloatCommit("X##scale", comp.scale.x);
        ImGui::SameLine();
        InputFloatCommit("Y##scale", comp.scale.y);
        ImGui::SameLine();
        InputFloatCommit("Z##scale", comp.scale.z);
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
            if (ImGui::CollapsingHeader(drawData.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                drawData.drawFunc(selected);
            }
        }
    }

    ImGui::EndChild();
    ImGui::End();
}
