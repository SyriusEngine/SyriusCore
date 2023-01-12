struct VSOut {
    float2 texCoords: TexCoord;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float2 texCoords : TexCoord){
	VSOut vso;
	float4 extPos = float4(pos, 1.0f);
	vso.pos = extPos;
	vso.texCoords = texCoords;
	return vso;
}