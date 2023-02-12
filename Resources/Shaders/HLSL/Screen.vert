struct VSOut {
	float2 texCoords: TexCoord;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float2 texCoords : TexCoord, uint iid: SV_InstanceID){
	VSOut vso;
	vso.pos = float4(pos, 1);
	vso.texCoords = texCoords;
	return vso;
}