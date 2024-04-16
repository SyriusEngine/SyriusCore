#version 420

layout(location = 0) in vec3 position;

layout (location = 16) out vec3 fTexCoord;

layout(std140, binding = 0) uniform ProjectionData {
    mat4 perspective;
    mat4 orthogonal;
};

layout(std140, binding = 1) uniform CameraData {
    mat4 view;
};

void main() {
    mat4 augmentedView = mat4(mat3(view));
    gl_Position = perspective * augmentedView * vec4(position, 1.0);
    fTexCoord = position;
}