struct FSin{
    float4 position: SV_Position;
    float3 texCoord: Texcoord;
};

struct FSout{
    float4 color: SV_Target0;
};

TextureCube cm: register(t0);

SamplerState splr: register(s0);

cbuffer LerpData: register(b4){
    float4 lerpFactor;
};

FSout main(FSin input){
    FSout output;
    output.color = cm.Sample(splr, input.texCoord);
    return output;
}