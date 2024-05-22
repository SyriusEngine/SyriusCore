#version 460

layout (location = 16) in vec4 fColor;
layout (location = 17) in vec2 fTexCoord;

layout (location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D ca1;

void main(){
   fragColor = texture(ca1, fTexCoord);
}