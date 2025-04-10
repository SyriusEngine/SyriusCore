#version 460

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lColor;
layout (location = 2) in vec3 lNormal;
layout (location = 3) in vec3 lTangent;
layout (location = 4) in vec2 lTexCoord;

layout (location = 16) out vec4 fColor;
layout (location = 17) out vec3 fNormal;
layout (location = 18) out vec3 fTangent;
layout (location = 19) out vec2 fTexCoord;

void main(){
    gl_Position = vec4(lPosition, 1.0);
    fColor = vec4(lColor, 1.0);
    fNormal = lNormal
    fTangent = lTangent;
    fTexCoord = lTexCoord;
}