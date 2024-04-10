#pragma once

#include <string>

static std::string s_GLSLVertexShaderSource = R"(
#version 420
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

out vec2 texCoords;

void main(){
    texCoords = textureCoords;
    gl_Position = vec4(position, 1.0);
}
)";

static std::string s_GLSLFragmentShaderSource = R"(
#version 420

in vec2 texCoords;

out vec4 FragColor;

layout(binding = 0) uniform sampler2D tex1;

void main(){
    vec4 tColor1 = texture(tex1, texCoords);
    if (tColor1.a < 0.1f){
        discard;
    }
    FragColor = tColor1;

}
)";

static std::string s_HLSLVertexShaderSource = R"(
struct VSOut {
    float2 texCoords: TexCoord;
    float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float2 texCoords : TexCoord){
    VSOut vso;
    vso.pos = float4(pos, 1.0f);
    vso.texCoords = texCoords;
    return vso;
}
)";

static std::string s_HLSLFragmentShaderSource = R"(
Texture2D tex1: register(t0);

SamplerState splr1: register(s0);

float4 main(float2 texCoords : TexCoord ) : SV_Target {
    float4 color1 = tex1.Sample(splr1, texCoords);
    if (color1.a < 0.1){
        discard;
    }
    return color1;
}
)";