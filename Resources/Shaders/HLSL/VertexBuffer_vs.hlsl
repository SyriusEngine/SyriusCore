struct VSin{
    float3 lPosition: Position;
    float3 lColor: Color;
    float3 lNormal: Normal;
    float3 lTangent: Tangent;
    float2 lTexCoord: TexCoord;
};

struct VSOut{
    float4 position: SV_Position;
    float4 color: Color;
};

VSOut main(VSin input){
    VSOut output;
    output.position = float4(input.lPosition, 1.0f);
    output.color = float4(input.lColor, 1.0f);
    return output;
}