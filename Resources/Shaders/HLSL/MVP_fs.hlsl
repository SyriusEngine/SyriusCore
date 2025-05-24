struct FSin{
    float4 position: SV_Position;
    float4 color: Color;
    float2 texCoord: Texcoord;
};

struct FSout{
    float4 color: SV_Target0;
};

FSout main(FSin input){
    FSout output;
    output.color = input.color;
    return output;
}