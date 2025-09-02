#include <imgui.h>

#include "para-dars/editor/panels/LogPanel.h"
#include "para-dars/core/LogManager.h"

LogPanel::LogPanel(Editor* editor) 
    : Panel(editor) {}

void LogPanel::OnImGuiRender() {
    ImGui::Begin("Log");

    ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), false , ImGuiWindowFlags_HorizontalScrollbar);
    const auto& logs = LogManager::GetLogs();
    for (const auto& entry : logs) {
        ImVec4 color;
        switch (entry.type) {
            case LogType::Info:    color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f); break;
            case LogType::Warning: color = ImVec4(1.0f, 0.8f, 0.2f, 1.0f); break;
            case LogType::Error:   color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); break;
        }

        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextWrapped("%s", entry.message.c_str());
        ImGui::PopStyleColor();
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();

    if (ImGui::Button("Clear Logs")) {
        LogManager::Clear();
    }

    ImGui::End();
}