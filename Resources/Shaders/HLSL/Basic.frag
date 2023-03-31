Texture2D tex1: register(t0);

SamplerState splr1: register(s0);

float4 main(float2 texCoords : TexCoord ) : SV_Target {
 	float4 color1 = tex1.Sample(splr1, texCoords);
 	if (color1.a < 0.1){
 	    discard;
 	}
 	return color1;
}