#pragma once

#include <string>

static std::string s_GLSLVertexShader = R"(
#version 460

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lColor;

void main(){
    gl_Position = vec4(lPosition, 1.0);
}
)";

static std::string s_GLSLFragmentShader1CA = R"(
#version 460

layout (location = 0) out vec4 fColorCA1;

void main(){
    fColorCA1 = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

static std::string s_GLSLFragmentShader2CA = R"(
#version 460

layout (location = 0) out vec4 fColorCA1;
layout (location = 1) out vec4 fColorCA2;

void main(){
    fColorCA1 = vec4(1.0, 0.0, 0.0, 1.0);
    fColorCA2 = vec4(0.0, 1.0, 0.0, 1.0);
}
)";




static std::string s_HLSLVertexShader = R"(
struct VsIn{
    float3 position : POSITION;
    float3 color : COLOR;
};

struct VsOut{
    float4 position : SV_POSITION;
};

VsOut main(VsIn input){
    VsOut output;
    output.position = float4(input.position, 1.0);
    return output;
}
)";

static std::string s_HLSLFragmentShader1CA = R"(
struct PsIn{
    float4 position : SV_POSITION;
};

struct PsOut{
    float4 color1 : SV_TARGET0;
};

PsOut main(PsIn input){
    PsOut output;
    output.color1 = float4(1.0, 0.0, 0.0, 1.0);
    return output;
}
)";

static std::string s_HLSLFragmentShader2CA = R"(
struct PsIn{
    float4 position : SV_POSITION;
};

struct PsOut{
    float4 color1 : SV_TARGET0;
    float4 color2 : SV_TARGET1;
};

PsOut main(PsIn input){
    PsOut output;
    output.color1 = float4(1.0, 0.0, 0.0, 1.0);
    output.color2 = float4(0.0, 1.0, 0.0, 1.0);
    return output;
}
)";
