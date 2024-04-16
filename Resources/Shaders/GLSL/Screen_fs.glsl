#version 460

layout (location = 16) in vec4 fColor;
layout (location = 17) in vec2 fTexCoord;

layout (location = 0) out vec4 fragColor;

uniform sampler2D tex;

layout(std140, binding = 4) uniform LerpData {
    vec4 lerpFactor;
};

void main(){
    vec4 texel = texture(tex, fTexCoord * lerpFactor.y);
    fragColor = mix(fColor, texel, lerpFactor.x);
}