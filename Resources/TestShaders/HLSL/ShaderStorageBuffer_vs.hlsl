struct VSin{
    float3 lPosition: Position;
    float3 lColor: Color;
    float2 lTexCoord: TexCoord;
};

struct VSOut{
    float4 position: SV_Position;
    float4 color: Color;
};

struct PositionDataStruct{
    float4 positionData[3];
    bool4 usePositionAsColor;
};

RWStructuredBuffer<PositionDataStruct> PositionData: register(u0);

VSOut main(VSin input){
    VSOut output;
    output.position = float4(input.lPosition, 1.0f);
    if (PositionData[0].usePositionAsColor.x){
        output.color = output.position;
    }
    else{
        output.color = float4(input.lColor, 1.0f);
    }
    output.color = float4(input.lColor, 1.0f);
    return output;
}