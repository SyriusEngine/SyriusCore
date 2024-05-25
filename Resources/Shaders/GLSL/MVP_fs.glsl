#version 460

layout (location = 16) in vec4 fColor;
layout (location = 17) in vec2 fTexCoord;

layout (location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D tex1;
void main(){
    vec4 texel1 = texture(tex1, fTexCoord);
    if (texel1.a < 0.1f) {
        discard;
    }
    fragColor = texel1;
}