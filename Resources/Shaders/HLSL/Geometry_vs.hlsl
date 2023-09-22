cbuffer ProjectionData: register(b0){
    matrix<float, 4, 4> perspective;
    matrix<float, 4, 4> orthogonal;
};

cbuffer CameraData: register(b1){
    matrix<float, 4, 4> view;
};

cbuffer ModelData: register(b2){
    matrix<float, 4, 4> modelMatrix;
}

struct VS_IN{
    float3 position: Position;
    float2 texCoords: TexCoord;
};

struct VS_OUT{
    float4 position: SV_POSITION;
    float2 texCoords: TexCoord;
};

VS_OUT main(VS_IN input){
    VS_OUT output;
    output.position = mul(mul(mul(float4(input.position, 1.0f), modelMatrix), view), perspective);
    output.texCoords = input.texCoords;
    return output;
}