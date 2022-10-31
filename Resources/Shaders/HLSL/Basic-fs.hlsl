Texture2D tex1: register(t0);
Texture2D tex2: register(t1);

SamplerState splr1: register(s0);
SamplerState splr2: register(s1);

float4 main(float2 texCoords : TexCoord ) : SV_Target {
	float4 color1 = tex1.Sample(splr1, texCoords * 2.0);
	float4 color2 = tex2.Sample(splr1, texCoords * 2.0);
	float4 color = lerp(color1, color2, 0.5);
	if (color.a < 0.1){
	    discard;
	}
	return color;
}