#pragma once

#include <glad/glad.h>

#include "para-dars/graphics/Shader.h"

struct Texture {
    unsigned int id;
    std::string type;
};

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float shininess;

    Texture diffuseMap;
    Texture specularMap;

    Material()
        : ambient(1.0f, 1.0f, 0.0f)   
        , diffuse(1.0f, 1.0f, 0.0f)   
        , specular(1.0f, 1.0f, 0.0f)  
        , shininess(32.0f)             
        , diffuseMap{ 0, "diffuse"}      
        , specularMap{ 0, "specular"}
    {}
    
    void Bind(Shader& shader) const {
        shader.SetVec3("material.ambient", ambient);
        shader.SetVec3("material.diffuse", diffuse);
        shader.SetVec3("material.specular", specular);
        shader.SetFloat("material.shininess", shininess);
        
        if (diffuseMap.id) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap.id);
            shader.SetInt("material.diffuseMap", 0);
            shader.SetBool("material.hasDiffuseMap", true);
        } else shader.SetBool("material.hasDiffuseMap", false);
        
        if (specularMap.id) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap.id);
            shader.SetInt("material.specularMap", 1);
            shader.SetBool("material.hasSpecularMap", true);
        } else shader.SetBool("material.hasSpecularMap", false);
    }
};