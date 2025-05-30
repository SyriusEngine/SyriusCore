#version 460

layout (location = 16) in vec4 fColor;
layout (location = 17) in vec2 fTexCoord;

layout (location = 0) out vec4 color1;
layout (location = 1) out vec4 color2; // inverted color

layout(binding = 0) uniform sampler2D tex1;
layout(binding = 1) uniform sampler2D tex2;

layout(std140, binding = 4) uniform TextureParameters {
    float lerpFactor;
    float scale;
    float offset[2];
};

void main(){
    vec4 texel1 = texture(tex1, fTexCoord * scale);
    vec4 texel2 = texture(tex2, fTexCoord * scale);
    vec4 texel = mix(texel1, texel2, lerpFactor);
    if (texel.a < 0.1f) {
        discard;
    }
    color1 = texel;
    color2 = vec4(1.0f) - texel;
}