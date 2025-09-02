#pragma once

#include <string>
#include <imgui.h>

inline bool InputTextCommit(const char* label, std::string& value, size_t bufSize = 256) {
    static std::string editBuffer;
    editBuffer.resize(bufSize);

    std::snprintf(editBuffer.data(), bufSize, "%s", value.c_str());

    bool committed = false;

    if (ImGui::InputText(label, editBuffer.data(), bufSize, ImGuiInputTextFlags_EnterReturnsTrue)) {
        value = editBuffer.c_str();
        committed = true;
    }
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        value = editBuffer.c_str(); 
        committed = true;
    }
    return committed;

}

inline bool InputFloatCommit(const char* label, float& value, const char* format = "%.3f") {
    float temp = value;
    bool committed = false;

    if (ImGui::InputFloat(label, &temp, 0, 0, format, ImGuiInputTextFlags_EnterReturnsTrue)) {
        value = temp;
        committed = true;
    }
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        value = temp;
        committed = true;
    }
    return committed;
}

inline bool InputIntCommit(const char* label, int& value) {
    int temp = value;
    bool committed = false;

    if (ImGui::InputInt(label, &temp, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue)) {
        value = temp;
        committed = true;
    }
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        value = temp;
        committed = true;
    }
    return committed;
}
