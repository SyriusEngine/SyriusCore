cbuffer Transform: register (b0){
    float4 translate[100];
}

struct VSOut {
    float2 texCoords: TexCoord;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float2 texCoords : TexCoord, uint iid: SV_InstanceID){
	VSOut vso;
	vso.pos = float4(pos.x + translate[iid].x, pos.y + translate[iid].y, pos.z,1.0f);
	vso.texCoords = texCoords;
	return vso;
}