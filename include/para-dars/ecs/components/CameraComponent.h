#pragma once

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "para-dars/ecs/components/TransformComponent.h"

struct CameraComponent {
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::mat4 GetProjectionMatrix(float aspectRatio) const {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }
};

struct Camera {
    TransformComponent transform;
    CameraComponent camera;
};