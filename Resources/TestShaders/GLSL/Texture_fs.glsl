#version 460

layout (location = 16) in vec4 fColor;
layout (location = 17) in vec2 fTexCoord;

layout (location = 0) out vec4 fragColor;

uniform sampler2D tex;

void main(){
    vec4 texel = texture(tex, fTexCoord * 2.0f);
    if (texel.a < 0.1f) {
        discard;
    }
    fragColor = mix(fColor, texel, 0.5f);
}