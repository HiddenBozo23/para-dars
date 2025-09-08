#pragma once

#include <cstdint>

#include "para-dars/ecs/Serialisable.h"

enum class RenderLayer : uint32_t {
    LAYER_DEFAULT = 1 << 0,
    LAYER_OPAQUE = 1 << 1,
    LAYER_TRANSPARENT = 1 << 2,
    LAYER_SKYBOX = 1 << 3,
    LAYER_UI = 1 << 4
};

struct RenderComponent : Serialisable {
    RenderLayer layer = RenderLayer::LAYER_DEFAULT;

    bool visible = true;

    std::string Serialise() override {
        nlohmann::json j;
        j["layer"] = layer;
        j["visible"] = static_cast<uint32_t>(visible);
        return j.dump();
    }

    void Deserialise(const std::string& data) override {
        nlohmann::json j = nlohmann::json::parse(data);
        layer = static_cast<RenderLayer>(j.value("layer", static_cast<uint32_t>(RenderLayer::LAYER_DEFAULT)));
        visible = j.value("visible", true);
    }
};