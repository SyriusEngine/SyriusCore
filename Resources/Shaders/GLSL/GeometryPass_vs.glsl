#version 460

layout(location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lColor;
layout (location = 2) in vec3 lNormal;
layout (location = 3) in vec3 lTangent;
layout (location = 4) in vec2 lTexCoord;

out VSOut{
    vec4 worldPosition;
    vec2 texCoords;
    mat3 tbn;
} vs_out;

layout(std140, binding = 0) uniform TransformData {
    mat4 transform;
};

layout(std140, binding = 1) uniform CameraData {
    mat4 view;
};

layout(std140, binding = 2) uniform ProjectionData {
    mat4 perspective;
};

void main() {
    mat3 truncNormalMatrix = mat3(transform);
    vec3 N = normalize(truncNormalMatrix * lNormal);
    vec3 T = normalize(truncNormalMatrix * lTangent);
    // re-orthogonalize T
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    vs_out.texCoords = lTexCoord;
    vs_out.tbn = mat3(T, B, N);
    vs_out.worldPosition = transform * vec4(lPosition, 1.0);

    gl_Position = perspective * view * vs_out.worldPosition;

}