#include <nfd.h>

#include "para-dars/core/FileDialogManager.h"

std::string FileDialogManager::OpenFile(const std::string& filterList, const std::string& defaultPath) {
    nfdchar_t* outPath = nullptr;
    nfdresult_t result = NFD_OpenDialogU8(&outPath, nullptr, 0, defaultPath.c_str());
    std::string selectedFile;

    if (result == NFD_OKAY) {
        selectedFile = outPath;
        free(outPath);
    }
    else if (result != NFD_CANCEL) {
        LogManager::Log(LogType::Error, "ERROR: OPEN DIALOG FAILED: " + std::string(NFD_GetError()));
    }
    return selectedFile;
}

std::string FileDialogManager::SaveFile(const std::string& filterList, const std::string& defaultPath, const std::string& defaultName) {
    nfdchar_t* outPath = nullptr;
    nfdresult_t result = NFD_SaveDialogU8(&outPath, nullptr, 0, defaultPath.c_str(), defaultName.c_str());
    std::string selectedFile;

    if (result == NFD_OKAY) {
        selectedFile = outPath;
        free(outPath);
    }
    else if (result != NFD_CANCEL) {
        LogManager::Log(LogType::Error, "ERROR: SAVE DIALOG FAILED: " + std::string(NFD_GetError()));
    }
    return selectedFile;
}