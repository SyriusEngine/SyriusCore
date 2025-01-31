struct PS_IN{
    float4 position: SV_Position;
    float4 worldPosition: WorldPosition;
    float2 texCoords: TexCoords;
    float3x3 TBN: TBNMatrix;
};

struct PS_OUT{
    float4 gPositions: SV_TARGET0;
    float4 gNormals : SV_TARGET1;
    float4 gAlbedo : SV_TARGET2;
    float4 gMrao : SV_TARGET3;
};

cbuffer ColorData: register(b4){
    float lerpFactor;
    float scale;
    float2 offset[2];
};

SamplerState splr: register(s0);

Texture2D albedoTex: register(t0);
Texture2D normalTex: register(t1);
Texture2D metallicTex: register(t2);
Texture2D roughnessTex: register(t3);
Texture2D aoTex: register(t4);

PS_OUT main(PS_IN psIn){
    float2 texCoords = psIn.texCoords * scale;

    float4 albedo = albedoTex.Sample(splr, texCoords);
    float4 normalTexel = normalTex.Sample(splr, texCoords);
    float4 metallic = metallicTex.Sample(splr, texCoords);
    float4 roughness = roughnessTex.Sample(splr, texCoords);
    float4 ao = aoTex.Sample(splr, texCoords);

    float3 transformedNormal = normalTexel.xyz * 2.0f - 1.0f;
    float3 normal = normalize(mul(transformedNormal, psIn.TBN));

    PS_OUT psOut;
    psOut.gPositions = psIn.worldPosition;
    psOut.gNormals = float4(normal, 0.0f);
    psOut.gAlbedo = albedo;
    psOut.gMrao = float4(metallic.r, roughness.r, ao.r, 0.0f);

    return psOut;
}