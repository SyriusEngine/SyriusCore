struct VSin{
    float3 lPosition: Position;
    float3 lColor: Color;
    float2 lTexCoord: TexCoord;
};

struct VSOut{
    float4 position: SV_Position;
    float4 color: Color;
    float2 texCoord: TexCoord;
};

cbuffer TransformData: register(b0){
    matrix<float, 4, 4> transform;
};

VSOut main(VSin input){
    VSOut output;
    output.position = mul(transform, float4(input.lPosition, 1.0f));
    output.color = float4(input.lColor, 1.0f);
    output.texCoord = input.lTexCoord;
    return output;
}