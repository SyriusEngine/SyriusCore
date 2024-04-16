#version 420

layout (location = 16) in vec3 fTexCoord;

layout (location = 0) out vec4 fragColor;

layout(binding = 0) uniform samplerCube cm;

void main() {
    fragColor = texture(cm, fTexCoord);
}