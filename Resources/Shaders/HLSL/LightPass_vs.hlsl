struct VS_IN{
    float3 lPosition: Position;
    float3 lColor: Color;
    float3 lNormal: Normal;
    float3 lTangent: Tangent;
    float2 lTexCoord: TexCoord;
};
struct VS_OUT{
    float4 position: SV_POSITION;
    float2 texCoords: TexCoords;
    float4 cameraPos: CameraPos;
};

cbuffer CameraData: register(b1){
    float4x4 view;
    float4 cameraPos;
};

VS_OUT main(VS_IN vsIn){
    VS_OUT vsOut;
    vsOut.position = float4(vsIn.lPosition, 1.0);
    vsOut.cameraPos = cameraPos;
    vsOut.texCoords = vsIn.lTexCoord;
    return vsOut;
}