Texture2D tex;

SamplerState splr;

float4 main(float2 texCoords : TexCoord ) : SV_Target {
	float4 color = tex.Sample(splr, texCoords);
	if (color.a < 0.1){
	    discard;
	}
	return color;
}