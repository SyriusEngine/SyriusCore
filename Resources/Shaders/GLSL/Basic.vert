#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

out vec2 texCoords;

layout(std140, binding = 0) uniform Transform {
    mat4 model;
};

void main(){
    texCoords = textureCoords;
    gl_Position = model * vec4(position, 1.0);
}