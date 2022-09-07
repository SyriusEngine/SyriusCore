#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

out vec2 texCoords;

layout(std140, binding = 0) uniform Transform {
    vec4 trans;
};

void main(){
    texCoords = textureCoords;
    gl_Position = vec4(position.xyz + trans.xyz, 1.0) ;
}