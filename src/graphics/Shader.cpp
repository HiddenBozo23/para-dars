#include <fstream>
#include <sstream>

#include "para-dars/graphics/Shader.h"
#include "para-dars/core/LogManager.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = LoadShaderSource(vertexPath);
    std::string fragmentCode = LoadShaderSource(fragmentPath);

    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        LogManager::Log(LogType::Error, "ERROR: FAILED TO LINK SHADER PROGRAM \n" + (std::string)infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::Use() {
    glUseProgram(id);
}

void Shader::SetBool(const std::string& name, bool value) {
    glUniform1i(GetUniformLocation(name), static_cast<int>(value));
}

void Shader::SetInt(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) {
    glUniform2fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat) {
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

std::string Shader::LoadShaderSource(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LogManager::Log(LogType::Error, "ERROR: FAILED TO FIND SHADER FILE " + path);
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::CompileShader(GLenum type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        LogManager::Log(LogType::Error, "ERROR: SHADER FAILED TO COMPILE \n" + (std::string)infoLog);
    }

    return shader;
}

unsigned int Shader::GetUniformLocation(const std::string& name) {
    auto it = uniformLocationCache.find(name);
    if (it != uniformLocationCache.end()) return it->second;

    unsigned int location = glGetUniformLocation(id, name.c_str());
    if (location == -1) {
        LogManager::Log(LogType::Error, "ERROR: SHADER FAILED TO FIND UNIFORM: " + name);
        return -1;
    }
    uniformLocationCache[name] = location;
    return location;
}