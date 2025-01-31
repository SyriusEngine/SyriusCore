struct PS_IN{
    float4 position: SV_POSITION;
    float2 texCoords: TexCoords;
    float4 cameraPos: CameraPos;
};

struct PS_OUT {
    float4 color : SV_Target0;
};

cbuffer LightData : register(b5) {
    float4 lightPos[300];
    float4 lightColor[300];
    uint4 lightCount;
}

Texture2D positions : register(t0);
Texture2D normals : register(t1);
Texture2D albedo : register(t2);
Texture2D mrao : register(t3);

SamplerState splr : register(s0);

static const float PI = 3.14159265359f;

float trowbridgeReitzGGX(float3 normal, float3 halfway, float roughness)
{
    float a2 = roughness * roughness;

    float t1 = max(dot(normal, halfway), 0.0f);
    float t2 = ((t1 * t1) * (a2 - 1) + 1);
    float denom = PI * t2 * t2;
    float retVal = a2 / denom;

    return retVal;
}

float geometrySmith(float3 normal, float3 viewDir, float3 lightDir, float roughness)
{
    float nv = max(dot(normal, viewDir), 0.0f);
    float nl = max(dot(normal, lightDir), 0.0f);

    float gSchlickGGXnv = nv / (nv * (1 - roughness) + roughness);
    float gSchlickGGXnl = nl / (nl * (1 - roughness) + roughness);
    float retVal = gSchlickGGXnv * gSchlickGGXnl;

    return retVal;
}

// Fresnel Equation (Used to describe ratio of reflected and refracted light)
float3 fresnelSchlick(float cosTheta, float3 f0)
{
    return f0 + (1.0f - f0) * pow(1.0f - cosTheta, 5.0f);
}

PS_OUT main(PS_IN psIn) {
    float2 texCoord = psIn.texCoords;

    float4 mraoTexel = mrao.Sample(splr, texCoord);
    if (mraoTexel.a < 0.1f) { // if the a channel is lower than 1.0f, discard the pixel as this contains garbage data
        discard;
    }

    float metallic = mraoTexel.r;
    float roughness = mraoTexel.g;
    float ao = mraoTexel.b;

    float3 position = positions.Sample(splr, texCoord).xyz;
    float3 normal = normals.Sample(splr, texCoord).rgb;
    float3 albedoTexel = albedo.Sample(splr, texCoord).rgb;

    PS_OUT psOut;

    float3 viewDir = normalize(psIn.cameraPos.xyz - position);

    float3 F0 = lerp(float3(0.04f, 0.4f, 0.4f), albedoTexel.rgb, metallic);

    float3 result = float3(0.0, 0.0, 0.0);
    for (uint i = 0; i < lightCount.x; i++) {
        float3 lightDir = normalize(lightPos[i].xyz - position);
        float3 halfway = normalize(lightDir + viewDir);
        float NdotL = max(dot(normal, lightDir), 0.0);

        float distance = length(lightPos[i].xyz - position);
        float attenuation = 1.0 / (distance * distance);
        float3 radiance = lightColor[i].rgb * attenuation;

        float NDF = trowbridgeReitzGGX(normal, halfway, roughness);
        float G = geometrySmith(normal, viewDir, lightDir, roughness);
        float3 F = fresnelSchlick(max(dot(halfway, viewDir), 0.0), F0);

        float3 kS = F;
        float3 kD = float3(1.0, 1.0, 1.0) - kS;
        kD *= 1.0 - metallic;

        float3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * NdotL + 0.0001;
        float3 specular = numerator / denominator;
        result += (kD * albedoTexel / PI + specular) * NdotL * radiance;
    }

    float3 ambient = float3(0.03, 0.03, 0.03) * albedoTexel * ao;
    float3 color = ambient + result;
    color = color / (color + float3(1.0, 1.0, 1.0));
    color = pow(abs(color), float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));

    psOut.color = float4(position, 1.0);
    return psOut;
}