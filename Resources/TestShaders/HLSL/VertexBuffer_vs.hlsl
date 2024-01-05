struct VSin{
    float3 lPosition: Position;
    float3 lColor: Color;
};

struct VSOut{
    float4 position: SV_Position;
    float3 color: Color;
};

VSOut main(VSin input){
    VSOut output;
    output.position = float4(input.lPosition, 1.0f);
    output.color = input.lColor;
    return output;
}