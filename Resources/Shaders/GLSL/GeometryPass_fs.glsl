#version 460

in VSOut{
    vec4 worldPosition;
    vec2 texCoords;
    mat3 tbn;
} fs_in;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedo;
layout(location = 3) out vec4 gMetallicRoughnessAO;

layout(binding = 0) uniform sampler2D albedoTex;
layout(binding = 1) uniform sampler2D normalTex;
layout(binding = 2) uniform sampler2D metallicTex;
layout(binding = 3) uniform sampler2D roughnessTex;
layout(binding = 4) uniform sampler2D aoTex;

layout(std140, binding = 4) uniform TextureParameters {
    float lerpFactor;
    float scale;
    float offset[2];
};

void main() {
    vec4 albedo = texture(albedoTex, fs_in.texCoords  * scale);
    vec4 normalTexel = texture(normalTex, fs_in.texCoords  * scale);
    vec4 metallic = texture(metallicTex, fs_in.texCoords  * scale);
    vec4 roughness = texture(roughnessTex, fs_in.texCoords  * scale);
    vec4 ao = texture(aoTex, fs_in.texCoords  * scale);

    vec3 transformedNormal = normalTexel.xyz * 2.0 - 1.0;
    vec3 normal = normalize(fs_in.tbn * transformedNormal);

    gPosition = fs_in.worldPosition;
    gNormal = vec4(normal, 1.0);
    gAlbedo = albedo;
    gMetallicRoughnessAO = vec4(metallic.r, roughness.r, ao.r, 1.0);
}