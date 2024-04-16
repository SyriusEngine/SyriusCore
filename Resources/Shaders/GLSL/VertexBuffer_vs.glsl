#version 460

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lColor;
layout (location = 2) in vec2 lTexCoord;

layout (location = 16) out vec4 fColor;

void main(){
    gl_Position = vec4(lPosition, 1.0);
    fColor = vec4(lColor, 1.0);
}