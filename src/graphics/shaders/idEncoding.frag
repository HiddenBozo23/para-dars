#version 330 core
layout(location = 0) out vec3 FragColor;

uniform int entityID;

vec3 encodeID(int id) {
    // Pack into 24-bit RGB
    float r = float((id & 0x000000FF)) / 255.0;
    float g = float((id & 0x0000FF00) >> 8) / 255.0;
    float b = float((id & 0x00FF0000) >> 16) / 255.0;
    return vec3(r, g, b);
}

void main() {
    FragColor = encodeID(entityID);
}