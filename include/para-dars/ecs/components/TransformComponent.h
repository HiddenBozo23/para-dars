#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

struct TransformComponent {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 GetMatrix() const {
        return glm::translate(glm::mat4(1.0), position)
                * glm::toMat4(glm::quat(rotation))
                * glm::scale(glm::mat4(1.0f), scale);
    }

    void SetFromMatrix(const glm::mat4& matrix) {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::quat orientation;

        glm::decompose(matrix, scale, orientation, position, skew, perspective);

        rotation = glm::eulerAngles(orientation);
    }
};