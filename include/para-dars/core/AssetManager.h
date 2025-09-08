#pragma once

#include <assimp/scene.h>

#include "para-dars/graphics/Model.h"

class AssetManager {
    public:
        static Model* LoadModel(const std::string& path);
        static bool WriteFile(const std::string& path, const std::string& data);
        static std::string ReadFile(const std::string& path);

    private:
        static std::unordered_map<std::string, std::shared_ptr<Model>> modelCache;

        static void ProcessNode(aiNode* node, const aiScene* scene, const std::string& directory, std::vector<MeshEntry>& meshEntries);
        static MeshEntry ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory);
        static Texture LoadMaterialTexture(aiMaterial* aiMat, aiTextureType type, const std::string& typeName, const std::string& directory);
        static unsigned int LoadTextureFromFile(const std::string& directory);
};