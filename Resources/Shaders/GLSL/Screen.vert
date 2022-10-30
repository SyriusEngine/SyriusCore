#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

out vec2 texCoords;

void main(){
    texCoords = textureCoords;
    gl_Position = vec4(position, 1.0);
}