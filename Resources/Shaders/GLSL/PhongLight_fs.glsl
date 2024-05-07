#version 460

layout (location = 16) in vec4 fColor;
layout (location = 17) in vec3 fNormal;
layout (location = 18) in vec4 fWorldPos;
layout (location = 19) in vec2 fTexCoord;

layout (location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D tex1;
layout(binding = 1) uniform sampler2D tex2;

layout(std140, binding = 4) uniform TextureParameters {
    float lerpFactor;
    float scale;
    float offset[2];
};

struct Light{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

layout(std140, binding = 5) uniform Lights{
    Light lights[4];
    uint32 lightCount;
    float padding[3];
};

void main(){
    vec4 texel1 = texture(tex1, fTexCoord * scale);
    vec4 texel2 = texture(tex2, fTexCoord * scale);
    vec4 texel = mix(texel1, texel2, lerpFactor);
    if (texel.a < 0.1f) {
        discard;
    }
    fragColor = texel;
}