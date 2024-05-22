struct FSin{
    float4 position: SV_Position;
    float2 texCoord: Texcoord;
};

struct FSout{
    float4 color: SV_Target0;
};

Texture2D ca1: register(t0);

SamplerState splr: register(s0);

FSout main(FSin input){
    input.texCoord.y = 1.0f - input.texCoord.y;
    FSout output;
    output.color = ca1.Sample(splr, input.texCoord);
    return output;
}