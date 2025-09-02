#pragma once

#include <memory>

#include "para-dars/graphics/Model.h"

struct ModelComponent {
    Model* model;

    ModelComponent(Model* model)
        : model(model) {}
};
