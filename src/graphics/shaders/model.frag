#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool hasDiffuseMap;
    bool hasSpecularMap;
    sampler2D diffuseMap;
    sampler2D specularMap;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 2

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;

uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform int numDirectionalLights;
uniform int numPointLights;
uniform int numSpotLights;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 GetTex(bool hasMap, sampler2D map, vec3 defaultVal);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    for (int i=0; i<numDirectionalLights; i++) 
        result += CalcDirectionalLight(directionalLights[i], norm, viewDir);
    for (int i=0; i<numPointLights; i++) 
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    for (int i=0; i<numSpotLights; i++) 
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);;

    vec3 diffuseTex = GetTex(material.hasDiffuseMap, material.diffuseMap, vec3(1.0));
    vec3 specularTex = GetTex(material.hasSpecularMap, material.specularMap, vec3(0.0));

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * diffuseTex;
    vec3 specular = light.specular * material.specular * spec * specularTex;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));

    vec3 diffuseTex = GetTex(material.hasDiffuseMap, material.diffuseMap, vec3(1.0));
    vec3 specularTex = GetTex(material.hasSpecularMap, material.specularMap, vec3(1.0));

    vec3 ambient = light.ambient * material.ambient * diffuseTex;
    vec3 diffuse = light.diffuse * diff * material.diffuse * diffuseTex;
    vec3 specular = light.specular * spec * material.specular * specularTex;

    return attenuation * (ambient + diffuse + specular);

}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 diffuseTex = GetTex(material.hasDiffuseMap, material.diffuseMap, vec3(1.0));
    vec3 specularTex = GetTex(material.hasSpecularMap, material.specularMap, vec3(1.0));

    vec3 ambient  = light.ambient * material.ambient * diffuseTex;
    vec3 diffuse  = light.diffuse * diff * material.diffuse * diffuseTex;
    vec3 specular = light.specular * spec * material.specular * specularTex;

    return intensity * attenuation * (ambient + diffuse + specular);
}

vec3 GetTex(bool hasMap, sampler2D map, vec3 defaultVal) {
    if (hasMap)
        return texture(map, TexCoords).rgb;
    return defaultVal;
}