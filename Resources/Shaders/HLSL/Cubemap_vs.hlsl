struct VSin{
    float3 lPosition: Position;
};

struct VSOut{
    float4 position: SV_Position;
    float3 texCoord: TexCoord;
};

cbuffer ProjectionData: register(b0){
    matrix<float, 4, 4> perspective;
    matrix<float, 4, 4> orthogonal;
};

cbuffer CameraData: register(b1){
    matrix<float, 4, 4> view;
};

VSOut main(VSin input){
    float3x3 rotationScale = (float3x3)view;

    // Create a new 4x4 matrix with the extracted rotation and scale, and no translation
    float4x4 augmentedView = float4x4(rotationScale[0], 0,
                                   rotationScale[1], 0,
                                   rotationScale[2], 0,
                                   0, 0, 0, 1);
    VSOut output;
    output.position = mul(augmentedView, float4(input.lPosition, 1.0f));
    output.position = mul(perspective, output.position);
    output.texCoord = input.lPosition;
    return output;
}