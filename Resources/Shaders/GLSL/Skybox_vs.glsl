#version 460

layout(location = 0) in vec3 position;

out vec3 texCoords;

layout(std140, binding = 0) uniform ProjectionData {
    mat4 perspective;
    mat4 orthogonal;
};

layout(std140, binding = 1) uniform CameraData {
    mat4 view;
};

void main() {
    texCoords = position;
    gl_Position = perspective * view * vec4(position, 1.0);

}