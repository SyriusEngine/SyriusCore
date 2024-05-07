struct VSin{
    float3 lPosition: Position;
    float3 lColor: Color;
    float3 lNormal: Normal;
    float3 lTangent: Tangent;
    float2 lTexCoord: TexCoord;
};

struct VSOut{
    float4 position: SV_Position;
    float2 texCoord: TexCoord;
};

cbuffer TransformData: register(b0){
    matrix<float, 4, 4> transform;
};

VSOut main(VSin input){
    VSOut output;
    output.position = mul(transform, float4(input.lPosition, 1.0f));
    output.texCoord = input.lTexCoord;
    return output;
}