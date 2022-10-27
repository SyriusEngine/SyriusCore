cbuffer Transform: register (b0){
    matrix<float, 4, 4> translate;
}

struct VSOut {
    float2 texCoords: TexCoord;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float2 texCoords : TexCoord, uint iid: SV_InstanceID){
	VSOut vso;
	float4 extPos = float4(pos, 1.0f);
	vso.pos = mul(extPos, translate);
	vso.texCoords = texCoords;
	return vso;
}