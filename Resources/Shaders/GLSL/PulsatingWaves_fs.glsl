#version 460

layout (location = 16) in vec4 fColor;
layout (location = 17) in vec3 fNormal;
layout (location = 18) in vec3 fTangent;
layout (location = 19) in vec2 fTexCoord;

layout (location = 0) out vec4 fragColor;

layout(std140, binding = 5) uniform Parameters {
    vec2 resolution;
    float time;
    float deltaTime;
};

vec3 palette(float t) {
    vec3 a = vec3(0.5f, 0.5f, 0.5f);
    vec3 b = vec3(0.5f, 0.5f, 0.5f);
    vec3 c = vec3(1.0f, 1.0f, 1.0f);
    vec3 d = vec3(0.263f, 0.416f, 0.557f);

    return a + b * cos(6.28318f * (c * t + d));
}

void main(){
    vec2 uv = (gl_FragCoord.xy * 2.0 - resolution) / resolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;
        float d = length(uv) * exp(-length(uv0));
        vec3 col = palette(length(uv0) + i*.4 + time * .4);

        d = sin(d*8. + time)/8.;
        d = abs(d);
        d = pow(0.01 / d, 1.2);
        finalColor += col * d;
    }

    fragColor = vec4(finalColor, 1.0);
}