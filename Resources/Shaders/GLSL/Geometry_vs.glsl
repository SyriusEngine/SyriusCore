#version 420

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

out vec2 texCoords;

layout(std140, binding = 0) uniform ProjectionData {
    mat4 perspective;
    mat4 orthogonal;
};

layout(std140, binding = 1) uniform CameraData {
    mat4 view;
};

layout(std140, binding = 2) uniform ModelData {
    mat4 model;
};

void main(){
    texCoords = textureCoords;
    gl_Position = perspective * view * model * vec4(position, 1.0);
}