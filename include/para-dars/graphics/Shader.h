#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <unordered_map>

class Shader {
    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();

        void Use();

        void SetBool(const std::string& name, bool value);
        void SetInt(const std::string& name, int value);
        void SetFloat(const std::string& name, float value);
        void SetVec2(const std::string& name, const glm::vec2& value);
        void SetVec3(const std::string& name, const glm::vec3& value);
        void SetVec4(const std::string& name, const glm::vec4& value);
        void SetMat3(const std::string& name, const glm::mat3& mat);
        void SetMat4(const std::string& name, const glm::mat4& mat);

    private:
        unsigned int id;
        std::unordered_map<std::string, GLint> uniformLocationCache;

        std::string LoadShaderSource(const std::string& path);
        unsigned int CompileShader(GLenum type, const std::string& source);
        unsigned int GetUniformLocation(const std::string& name);
};