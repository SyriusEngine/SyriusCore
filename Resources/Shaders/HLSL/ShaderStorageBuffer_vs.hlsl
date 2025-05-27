struct VSin{
    float3 lPosition: Position;
    float3 lColor: Color;
    float3 lNormal: Normal;
    float3 lTangent: Tangent;
    float2 lTexCoord: TexCoord;
    uint instanceID: SV_InstanceID;
};

struct VSOut{
    float4 position: SV_Position;
    float4 color: Color;
    float2 texCoord: TexCoord;
};

struct Transform{
    matrix<float, 4, 4> transform;
};

StructuredBuffer<Transform> TransformData: register(t5);

cbuffer CameraData: register(b1){
    matrix<float, 4, 4> view;
};

cbuffer ProjectionData: register(b2){
    matrix<float, 4, 4> perspective;
    matrix<float, 4, 4> orthogonal;
};

VSOut main(VSin input){
    VSOut output;
    output.position = mul(TransformData[input.instanceID].transform, float4(input.lPosition, 1.0f));
    output.position = mul(view, output.position);
    output.position = mul(perspective, output.position);
    output.color = float4(input.lColor, 1.0f);
    output.texCoord = input.lTexCoord;
    return output;
}