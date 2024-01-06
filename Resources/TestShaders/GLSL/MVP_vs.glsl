#version 420

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 lColor;
layout (location = 2) in vec2 lTexCoord;

layout (location = 16) out vec4 fColor;
layout (location = 17) out vec2 fTexCoord;

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
    fColor = vec4(lColor, 1.0);
    fTexCoord = lTexCoord;
    gl_Position = perspective * view * model * vec4(position, 1.0);
}