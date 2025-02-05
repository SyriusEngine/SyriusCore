#version 460

layout (location = 16) in vec4 fColor;
layout (location = 17) in mat3 fNormal;

layout (location = 0) out vec4 fragColor;

void main(){
    fragColor = fColor;
}