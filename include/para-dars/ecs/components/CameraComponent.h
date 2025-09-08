#pragma once

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "para-dars/ecs/Serialisable.h"
#include "para-dars/ecs/components/TransformComponent.h"

struct CameraComponent : Serialisable {
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::mat4 GetProjectionMatrix(float aspectRatio) const {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    std::string Serialise() override {
        nlohmann::json j;
        j["fov"] = fov;
        j["nearPlane"] = nearPlane;
        j["farPlane"] = farPlane;
        return j.dump();
    }

    void Deserialise(const std::string& data) override {
        nlohmann::json j = nlohmann::json::parse(data);
        fov = j.value("fov", 45.0f);
        nearPlane = j.value("nearPlane", 0.1f);
        farPlane  = j.value("farPlane", 100.0f);
    }
};

struct Camera {
    TransformComponent transform;
    CameraComponent camera;
};