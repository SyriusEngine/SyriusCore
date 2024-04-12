#pragma once

#include <string>

static std::string s_GLSLVertexShaderSource = R"(
#version 460
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;

out vec2 texCoords;

void main(){
    texCoords = textureCoords;
    gl_Position = vec4(position, 1.0);
}
)";

static std::string s_GLSLFragmentShaderSource = R"(
#version 460

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

static std::string s_GlslVertexShaderVertexSSBOSource = R"(
#version 460

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lColor;

out vec4 fColor;

layout(std430, binding = 0) buffer PositionData{
    vec4 positionData[3];
    vec4 offset;
    bvec4 readPositionFromBuffer;
};

void main(){
    fColor = vec4(lColor, 1.0f);
    vec4 pos = vec4(lPosition, 1.0);
    if (readPositionFromBuffer.x){
        pos = positionData[gl_VertexID];
    }
    pos += offset;
    positionData[gl_VertexID] = pos;
    gl_Position = pos;
}
)";

static std::string s_GlslFragmentShaderVertexSSBOSource = R"(
#version 460

in vec4 fColor;

layout(location = 0) out vec4 lFragColor;

void main(){
    lFragColor = fColor;
}
)";

static std::string s_GlslVertexShaderFragmentSSBOSource = R"(
#version 460

layout (location = 0) in vec3 lPosition;
layout (location = 1) in vec3 lColor;

out vec4 fColor;

void main(){
    fColor = vec4(lColor, 1.0f);
    gl_Position = vec4(lPosition, 1.0);
}
)";

static std::string s_GlslFragmentShaderFragmentSSBOSource = R"(
#version 460

in vec4 fColor;

layout(location = 0) out vec4 lFragColor;

layout(std430, binding = 0) coherent buffer ColorData{
    vec4 colorData;
    ivec4 increment;
    bvec4 readColorFromBuffer;
};

void main(){
    lFragColor = fColor;
    memoryBarrierBuffer();
    if (readColorFromBuffer.x){
        lFragColor = colorData;
    }
    else{
        colorData = lFragColor;
    }
    atomicAdd(increment.x, 1);

    memoryBarrierBuffer();
}
)";