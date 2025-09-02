#pragma once

#include <memory>

#include "para-dars/graphics/Mesh.h"
#include "para-dars/graphics/Shader.h"
#include "para-dars/graphics/Material.h"

struct MeshEntry {
    Mesh mesh;
    Material material;
};

class Model {
    public:
        Model(const std::vector<MeshEntry>& meshEntries);

        void Draw(Shader& shader, bool useMaterial = true);

    private:
        std::vector<MeshEntry> meshEntries;
};