struct VSin{
    float3 lPosition: Position;
    float3 lColor: Color;
    float3 lNormal: Normal;
    float3 lTangent: Tangent;
    float2 lTexCoord: TexCoord;
};

struct VSOut{
    float4 position: SV_Position;
    float3 texCoord: TexCoord;
};

cbuffer TransformData: register(b0){
    matrix<float, 4, 4> transform;
};

cbuffer CameraData: register(b1){
    matrix<float, 4, 4> view;
};

cbuffer ProjectionData: register(b2){
    matrix<float, 4, 4> perspective;
    matrix<float, 4, 4> orthogonal;
};

VSOut main(VSin input){
    VSOut output;
    output.position = mul(transform, float4(input.lPosition, 1.0f));
    output.position = mul(view, output.position);
    output.position = mul(perspective, output.position);
    output.texCoord = input.lPosition;
    return output;
}