#version 460

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lColor;
layout (location = 2) in vec3 lNormal;
layout (location = 3) in vec3 lTangent;
layout (location = 4) in vec2 lTexCoord;

out VSOut{
    vec2 texCoords;
    vec4 cameraPos;
} vsOut;

layout(std140, binding = 1) uniform CameraData {
    mat4 view;
    vec4 cameraPos;
};

void main() {
    gl_Position = vec4(lPosition, 1.0);
    vsOut.texCoords = lTexCoord;
    vsOut.cameraPos = cameraPos;
}