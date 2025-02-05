struct FSin{
    float4 position: SV_Position;
    float4 color: Color;
    float3 normal: Normal;
    float3 tangent: Tangent;
    float2 texCoord: TexCoord;
};

struct FSout{
    float4 color: SV_Target0;
};

FSout main(FSin input){
    FSout output;
    output.color = input.color;
    return output;
}
