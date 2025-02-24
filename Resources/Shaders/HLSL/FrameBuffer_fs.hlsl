struct FSin{
    float4 position: SV_Position;
    float4 color: Color;
    float2 texCoord: Texcoord;
};

struct FSout{
    float4 color1: SV_Target0;
    float4 color2: SV_Target1;
};

Texture2D tex1: register(t0);
Texture2D tex2: register(t1);

SamplerState splr: register(s0);

cbuffer ColorData: register(b4){
    float lerpFactor;
    float scale;
    float2 offset[2];
};

FSout main(FSin input){
    FSout output;
    float4 texel1 = tex1.Sample(splr, input.texCoord * scale);
    float4 texel2 = tex2.Sample(splr, input.texCoord * scale);
    float4 texel = lerp(texel1, texel2, lerpFactor);
    if (texel.a < 0.1f){
        discard;
    }
    output.color1 = texel;
    output.color2 = float4(1.0f, 0.0f, 0.0f, 1.0f) - texel;
    return output;
}