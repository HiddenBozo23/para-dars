#include "para-dars/graphics/Model.h"

Model::Model(const std::vector<MeshEntry>& meshEntries) 
    : meshEntries(meshEntries) {}

void Model::Draw(Shader& shader, bool useMaterial) {
    for (const auto& meshEntry : meshEntries) {
        if (useMaterial) meshEntry.material.Bind(shader);
        meshEntry.mesh.Draw();
    }
}