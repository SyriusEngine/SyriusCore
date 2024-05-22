#version 420

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 lColor;
layout (location = 2) in vec3 lNormal;
layout (location = 3) in vec3 lTangent;
layout (location = 4) in vec2 lTexCoord;

layout (location = 16) out vec4 fColor;
layout (location = 17) out vec3 fNormal;
layout (location = 18) out vec4 fWorldPos;
layout (location = 19) out vec2 fTexCoord;

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
    fColor = vec4(lColor, 1.0);
    fTexCoord = lTexCoord;
    fNormal = lNormal;
    fWorldPos = transform * vec4(position, 1.0);
    gl_Position = perspective * view * fWorldPos;
}