struct VS_IN{
    float3 lPosition: Position;
    float3 lColor: Color;
    float3 lNormal: Normal;
    float3 lTangent: Tangent;
    float2 lTexCoord: TexCoord;
};

struct VS_OUT{
    float4 position: SV_Position;
    float4 worldPosition: WorldPosition;
    float2 texCoords: TexCoords;
    float3x3 TBN: TBNMatrix;
};

cbuffer TransformData: register(b0){
    matrix<float, 4, 4> transform;
};

cbuffer CameraData: register(b1){
    matrix<float, 4, 4> view;
    float4 cameraPos;
};

cbuffer ProjectionData: register(b2){
    matrix<float, 4, 4> perspective;
    matrix<float, 4, 4> orthogonal;
};

VS_OUT main(VS_IN vsIn){
    matrix<float, 3, 3> truncNormalMatrix = (float3x3) transform;
    float3 N = normalize(mul(truncNormalMatrix, vsIn.lNormal));
    float3 T = normalize(mul(truncNormalMatrix, vsIn.lTangent));
    T = normalize(T - dot(T, N) * N); // Gram-Schmidt orthogonalization
    float3 B = cross(N, T);

    VS_OUT vsOut;
    vsOut.worldPosition = mul(transform, float4(vsIn.lPosition, 1.0));
    vsOut.TBN = float3x3(T, B, N);
    vsOut.texCoords = vsIn.lTexCoord;

    float4 viewPos = mul(view, vsOut.worldPosition);
    vsOut.position = mul(perspective, viewPos);

    return vsOut;
}

