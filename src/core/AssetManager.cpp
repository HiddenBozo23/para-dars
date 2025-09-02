#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb_image.h>

#include "para-dars/core/AssetManager.h"
#include "para-dars/core/LogManager.h"

std::unordered_map<std::string, std::shared_ptr<Model>> AssetManager::modelCache;

Model* AssetManager::LoadModel(const std::string& path) {
    if (modelCache.find(path) != modelCache.end()) return modelCache[path].get();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LogManager::Log(LogType::Error, "ERROR: NO MODEL FOUND AT " + path + ": " + importer.GetErrorString());
        return nullptr;
    }
    std::vector<MeshEntry> meshEntries;
    ProcessNode(scene->mRootNode, scene, path, meshEntries);

    auto model = std::make_shared<Model>(meshEntries);
    modelCache[path] = model;
    return model.get();
}

void AssetManager::ProcessNode(aiNode* node, const aiScene* scene, const std::string& path, std::vector<MeshEntry>& meshEntries) {
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshEntries.push_back(ProcessMesh(mesh, scene, path));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, path, meshEntries);
    }
}

MeshEntry AssetManager::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& path) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        vertex.normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );

        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        } else {
            vertex.texCoords = glm::vec2(0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D col;
        float shininess;

        if (aiMat->Get(AI_MATKEY_COLOR_AMBIENT, col) == AI_SUCCESS) 
            material.ambient = glm::vec3(col.r, col.g, col.b);
        if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, col) == AI_SUCCESS)
            material.diffuse = glm::vec3(col.r, col.g, col.b);
        if (aiMat->Get(AI_MATKEY_COLOR_SPECULAR, col) == AI_SUCCESS)
            material.specular = glm::vec3(col.r, col.g, col.b);
        
        size_t dirSlash = path.find_last_of("/\\");
        std::string workingDir = (dirSlash == std::string::npos) ? path : path.substr(0, dirSlash);

        material.diffuseMap = LoadMaterialTexture(aiMat, aiTextureType_DIFFUSE, "diffuse", workingDir);
        material.specularMap = LoadMaterialTexture(aiMat, aiTextureType_SPECULAR, "specular", workingDir);
    }

    return MeshEntry{Mesh{vertices, indices}, material};
}

Texture AssetManager::LoadMaterialTexture(aiMaterial* aiMaterial, aiTextureType type, const std::string& typeName, const std::string& directory) {
    Texture texture = {0, typeName};
    if (aiMaterial->GetTextureCount(type) > 0) {
        aiString str;
        aiMaterial->GetTexture(type, 0, &str);
        std::string filePath = directory + "/" + str.C_Str();

        texture.id = LoadTextureFromFile(filePath);
        texture.type = typeName;
    }
    return texture;
}

unsigned int AssetManager::LoadTextureFromFile(const std::string& directory) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(directory.c_str(), &width, &height, &nrComponents, 0);
    if (!data) {
        LogManager::Log(LogType::Error, "Error: failed to load texture at path: " + directory);
        return 0;
    }

    GLenum format = GL_RGB;
    if (nrComponents == 1) format = GL_RED;
    else if (nrComponents == 3) format = GL_RGB;
    else if (nrComponents == 4 ) format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureID;
}