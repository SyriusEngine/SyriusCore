#version 420

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 lColor;
layout (location = 2) in vec3 lNormal;
layout (location = 3) in vec3 lTangent;
layout (location = 4) in vec2 lTexCoord;

layout (location = 16) out vec3 fTexCoord;

layout(std140, binding = 0) uniform TransformData {
    mat4 transform;
};

layout(std140, binding = 1) uniform CameraData {
    mat4 view;
};

layout(std140, binding = 2) uniform ProjectionData {
    mat4 perspective;
    mat4 orthogonal;
};

void main(){
    fTexCoord = position;
    gl_Position = perspective * view * transform * vec4(position, 1.0);
}