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
