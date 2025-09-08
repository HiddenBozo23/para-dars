#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "para-dars/ecs/Serialisable.h"

struct TransformComponent : Serialisable {
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

    std::string Serialise() override {
        nlohmann::json j;
        j["position"] = { position.x, position.y, position.z };
        j["rotation"] = { rotation.x, rotation.y, rotation.z };
        j["scale"]    = { scale.x, scale.y, scale.z };
        return j.dump();
    }

    void Deserialise(const std::string& data) override {
        nlohmann::json j = nlohmann::json::parse(data);
        auto pos = j.value("position", std::vector<float>{0,0,0});
        auto rot = j.value("rotation", std::vector<float>{0,0,0});
        auto scl = j.value("scale", std::vector<float>{1,1,1});

        position = glm::vec3(pos[0], pos[1], pos[2]);
        rotation = glm::vec3(rot[0], rot[1], rot[2]);
        scale    = glm::vec3(scl[0], scl[1], scl[2]);
    }
};