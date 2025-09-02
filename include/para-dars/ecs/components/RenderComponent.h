#pragma once

#include <cstdint>

enum class RenderLayer : uint32_t {
    LAYER_DEFAULT = 1 << 0,
    LAYER_OPAQUE = 1 << 1,
    LAYER_TRANSPARENT = 1 << 2,
    LAYER_SKYBOX = 1 << 3,
    LAYER_UI = 1 << 4
};

struct RenderComponent {
    RenderLayer layer = RenderLayer::LAYER_DEFAULT;

    bool visible = true;
};