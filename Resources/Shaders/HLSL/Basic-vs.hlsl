cbuffer Transform: register (b0){
    float4 translate;
}

float4 main(float3 pos: Position): SV_Position {
    return float4(pos.x + translate.x, pos.y + translate.y, 0, 1);
}