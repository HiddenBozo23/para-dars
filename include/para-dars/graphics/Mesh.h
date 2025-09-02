#pragma once

#include <glm/glm.hpp>

#include <vector>


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        void Draw() const;

    private:    
        unsigned int VAO, VBO, EBO;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        void SetupMesh();
};