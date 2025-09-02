#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D mask;

uniform vec3 outlineColor = vec3(1.0, 1.0, 0.0);
uniform float edgeThreshold = 0.1;

void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(mask, 0));

    float maskCenter = texture(mask, TexCoords).r;
    float maskUp = texture(mask, TexCoords + vec2(0.0, texelSize.y)).r;
    float maskDown = texture(mask, TexCoords - vec2(0.0, texelSize.y)).r;
    float maskLeft = texture(mask, TexCoords - vec2(texelSize.x, 0.0)).r;
    float maskRight = texture(mask, TexCoords + vec2(texelSize.x, 0.0)).r;

    bool isEdge = maskCenter < 0.5 && (maskUp > edgeThreshold || maskDown > edgeThreshold || maskLeft > edgeThreshold || maskRight > edgeThreshold);

    vec3 sceneColor = texture(scene, TexCoords).rgb;

    if (isEdge)
        FragColor = vec4(outlineColor, 1.0);
    else
        FragColor = vec4(sceneColor, 1.0);
}