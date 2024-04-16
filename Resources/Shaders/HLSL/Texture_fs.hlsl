struct FSin{
    float4 position: SV_Position;
    float4 color: Color;
    float2 texCoord: Texcoord;
};

struct FSout{
    float4 color: SV_Target0;
};

Texture2D tex: register(t0);

SamplerState splr: register(s0);

FSout main(FSin input){
    FSout output;
    float4 texel = tex.Sample(splr, input.texCoord);
    if (texel.a < 0.1f){
        discard;
    }
    output.color = texel;
    return output;
}
