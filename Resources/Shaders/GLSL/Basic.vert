#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

out vec2 texCoords;

layout(std140, binding = 0) uniform Transformation {
    vec4 translation[100];
};

void main(){
    texCoords = textureCoords;
    gl_Position = vec4(position + translation[gl_InstanceID].xyz, 1.0) ;
}