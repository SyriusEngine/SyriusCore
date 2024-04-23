struct FSin{
    float4 position: SV_Position;
    float4 color: Color;
    float2 texCoord: Texcoord;
};

struct FSout{
    float4 color: SV_Target0;
};

cbuffer ColorData: register(b1){
    float4 color;
    float4 lerpFactor;
};

FSout main(FSin input){
    FSout output;
    output.color = lerp(input.color, color, lerpFactor.x);
    return output;
}