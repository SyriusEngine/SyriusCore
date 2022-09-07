Texture2D tex: register(t0); // Texture

SamplerState splr: register(s0); // Sampler

float4 main(float2 texCoords: TexCoords): SV_Target {
    float4 color = tex.Sample(splr, texCoords);
    return color;
}