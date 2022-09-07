struct SharedResource{
    float2 texCoords: TexCoords;
    float4 pos: SV_Position;
};

cbuffer Transform: register (b0){
    float4 translate;
}

SharedResource main(float3 pos: Position, float2 texCoords: TexCoords) {
    SharedResource output;
    output.pos = float4(pos.x + translate.x, pos.y + translate.y, 0, 1);
    output.texCoords = texCoords;
    return output;
}