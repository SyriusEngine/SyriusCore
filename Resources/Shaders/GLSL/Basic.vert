#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

out vec2 texCoords;

layout(std140, binding = 0) uniform Transform {
    vec2 trans[100];
};

void main(){
    texCoords = textureCoords;
    gl_Position = vec4(position.xy + trans[gl_InstanceID], 0.0, 1.0);
}