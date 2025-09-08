#pragma once

#include <string>

#include "LogManager.h"

class FileDialogManager {
    public: 
        static std::string OpenFile(const std::string& filterList = "", const std::string& defaultPath = "");
        static std::string SaveFile(const std::string& filterList = "", const std::string& defaultPath = "", const std::string& defaultName = "");
};