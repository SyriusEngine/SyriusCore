cbuffer Transform: register (b0){
    float4 translate;
}

struct VSOut {
    float2 texCoords: TexCoord;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float2 texCoords : TexCoord ) {
	VSOut vso;
	vso.pos = float4(pos.x + translate.x, pos.y + translate.y, pos.z,1.0f);
	vso.texCoords = texCoords;
	return vso;
}