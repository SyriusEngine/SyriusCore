#version 460

in VSOut{
    vec2 texCoords;
    vec4 cameraPos;
} fsIn;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedo;
layout(binding = 3) uniform sampler2D gMetallicRoughnessAO;

layout(std140, binding = 5) uniform LightData {
    vec4 lightPos[300];
    vec4 lightColor[300];
    uvec4 lightCount;
};

const float PI = 3.14159265359f;

float trowbridgeReitzGGX(vec3 normal, vec3 halfway, float roughness){
    float a2 = roughness * roughness;

    float t1 = max(dot(normal, halfway), 0.0f);
    float t2 = ((t1 * t1) * (a2 - 1) + 1);
    float denom = PI * t2 * t2;
    float retVal = a2 / denom;

    return retVal;
}

// GGX and Schlick-Beckmann Geometry function implemented with Smith's method
float geometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness){
    float nv = max(dot(normal, viewDir), 0.0f);
    float nl = max(dot(normal, lightDir), 0.0f);

    float gSchlickGGXnv = nv / (nv * (1 - roughness) + roughness);
    float gSchlickGGXnl = nl / (nl * (1 - roughness) + roughness);
    float retVal = gSchlickGGXnv * gSchlickGGXnl;

    return retVal;
}

// Fresnel Equation (Used to describe ratio of reflected and refracted light)
vec3 fresnelSchlick(float cosTheta, vec3 f0){
    return f0 + (1.0f - f0) * pow(1.0f - cosTheta, 5.0f);
}

void main(){
    vec4 mraoTexel = texture(gMetallicRoughnessAO, fsIn.texCoords);

    float metallic = mraoTexel.r;
    float roughness = mraoTexel.g;
    float ao = mraoTexel.b;

    vec3 position = texture(gPosition, fsIn.texCoords).xyz;
    vec3 normal = texture(gNormal, fsIn.texCoords).rgb;
    vec3 albedoTexel = texture(gAlbedo, fsIn.texCoords).rgb;

    vec3 viewDir = normalize(fsIn.cameraPos.xyz - position);

    vec3 F0 = mix(vec3(0.04), albedoTexel, metallic);

    vec3 result = vec3(0.0);
    for (uint i = 0; i < lightCount.x; i++){
        vec3 lightDir = normalize(lightPos[i].xyz - position);
        vec3 halfway = normalize(lightDir + viewDir);
        float NdotL = max(dot(normal, lightDir), 0.0);

        float distance = length(lightPos[i].xyz - position);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColor[i].rgb * attenuation;

        float NDF = trowbridgeReitzGGX(normal, halfway, roughness);
        float G = geometrySmith(normal, viewDir, lightDir, roughness);
        vec3 F = fresnelSchlick(max(dot(halfway, viewDir), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * NdotL + 0.0001;
        vec3 specular = numerator / denominator;
        result += (kD * albedoTexel / PI + specular) * NdotL * radiance;
    }

    vec3 ambient = vec3(0.03) * albedoTexel * ao;
    vec3 color = ambient + result;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    outColor = vec4(color, 1.0);
}