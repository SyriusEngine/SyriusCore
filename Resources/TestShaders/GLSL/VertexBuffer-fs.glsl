#version 460

layout (location = 16) in vec3 fColor;

layout (location = 0) out vec4 fragColor;

void main(){
    fragColor = vec4(fColor, 1.0);

}