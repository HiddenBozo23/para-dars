#pragma once

#include <memory>

#include "para-dars/graphics/Model.h"
#include "para-dars/ecs/Serialisable.h"
#include "para-dars/core/AssetManager.h"

struct ModelComponent : Serialisable {
    std::string path;
    Model* model;

    ModelComponent()
        : path(""), model(nullptr) {}

    ModelComponent(const std::string& path)
        : path(path) {
        model = AssetManager::LoadModel(path);
    }

    std::string Serialise() override {
        nlohmann::json j;
        j["path"] = path;
        return j.dump();
    }

    void Deserialise(const std::string& data) override {
        nlohmann::json j = nlohmann::json::parse(data);
        path = j.value("path", "");
        if (path != "")
            model = AssetManager::LoadModel(path);
    }
};
