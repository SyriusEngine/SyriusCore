#version 460

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 lColor;
layout (location = 2) in vec3 lNormal;
layout (location = 3) in vec3 lTangent;
layout (location = 4) in vec2 lTexCoord;

layout (location = 16) out vec4 fColor;
layout (location = 17) out vec2 fTexCoord;

layout(std140, binding = 0) uniform TransformData {
    mat4 transform;
};

void main() {
    gl_Position = transform * vec4(position, 1.0);
    fColor = vec4(lColor, 1.0);
    fTexCoord = lTexCoord;
}
