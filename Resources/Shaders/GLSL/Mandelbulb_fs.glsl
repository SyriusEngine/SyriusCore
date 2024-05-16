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

// #define phase_shift_on

float stime, ctime;
void ry(inout vec3 p, float a){
    vec3 q = p;
    float c = cos(a);
    float s = sin(a);
    p.x = c * q.x + s * q.z;
    p.z = -s * q.x + c * q.z;
}

float pixel_size = 0.0f;

vec3 mb(vec3 p) {
    p.xyz = p.xzy;
    vec3 z = p;
    vec3 dz = vec3(0.0f);
    float power = 8.0f;
    float r = 0.0f;
    float theta = 0.0f;
    float phi = 0.0f;
    float dr = 1.0;

    float t0 = 1.0f;
    for(int i = 0; i < 7; ++i) {
        r = length(z);
        if (r > 2.0f){
            continue;
        }
        theta = atan(z.y / z.x);
        #ifdef phase_shift_on
        phi = asin(z.z / r) + time * 0.1;
        #else
        phi = asin(z.z / r);
        #endif

        dr = pow(r, power - 1.0) * dr * power + 1.0;

        r = pow(r, power);
        theta = theta * power;
        phi = phi * power;

        z = r * vec3(cos(theta) * cos(phi), sin(theta) * cos(phi), sin(phi)) + p;

        t0 = min(t0, r);
    }
    return vec3(0.5 * log(r) * r / dr, t0, 0.0);
}

vec3 f(vec3 p){
    ry(p, time * 0.2f);
    return mb(p);
}


float softshadow(vec3 ro, vec3 rd, float k){
    float akuma = 1.0f;
    float h = 0.0f;
    float t = 0.01;
    for (int i=0; i < 50; ++i){
        h = f(ro + rd * t).x;
        if (h < 0.001f){
            return 0.2f;
        }
        akuma = min(akuma, k * h / t);
        t += clamp(h, 0.01f, 2.0f);
    }
    return akuma;
}

vec3 nor(vec3 pos) {
    vec3 eps = vec3(0.001f, 0.0f, 0.0f);
    return normalize( vec3(
        f(pos+eps.xyy).x - f(pos-eps.xyy).x,
        f(pos+eps.yxy).x - f(pos-eps.yxy).x,
        f(pos+eps.yyx).x - f(pos-eps.yyx).x
    ));
}

vec3 intersect(vec3 ro, vec3 rd){
    float t = 1.0f;
    float res_t = 0.0f;
    float res_d = 1000.0f;
    vec3 c, res_c;
    float max_error = 1000.0f;
    float d = 1.0f;
    float pd = 100.0f;
    float os = 0.0f;
    float step = 0.0f;
    float error = 1000.0f;

    for(int i = 0; i < 48; i++) {
        if (error > pixel_size * 0.5 && t < 20.0 ){
            c = f(ro + rd * t);
            d = c.x;
            if(d > os){
                os = 0.4 * d * d / pd;
                step = d + os;
                pd = d;
            }
            else {
                step =-os;
                os = 0.0;
                pd = 100.0;
                d = 1.0;
            }
            error = d / t;
            if (error < max_error) {
                max_error = error;
                res_t = t;
                res_c = c;
            }
            t += step;
        }
    }
    if (t > 20.0f) {
        res_t = -1.0f;
    }
    return vec3(res_t, res_c.y, res_c.z);
}


void main(){
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 q=fragCoord / resolution;
    vec2 uv = -2.0f + 2.0f * q * 2.0f;
    uv.x *= resolution.x / resolution.y;

    pixel_size = 1.0f / (resolution.x * 3.0f);
    // camera
    stime = 0.7f + 0.3f * sin(time * 0.4f);
    ctime = 0.7f + 0.3f * cos(time * 0.4f);

    vec3 ta = vec3(0.0f, 0.0f, 0.0f);
    vec3 ro = vec3(0.0f, 3.0f * stime * ctime, 3.0f * (1.0f - stime * ctime));

    vec3 cf = normalize(ta - ro);
    vec3 cs = normalize(cross(cf, vec3(0.0f, 1.0f, 0.0f)));
    vec3 cu = normalize(cross(cs, cf));
    vec3 rd = normalize(uv.x * cs + uv.y * cu + 3.0f * cf);  // transform from view to world

    vec3 sundir = normalize(vec3(0.1, 0.8, 0.6));
    vec3 sun = vec3(1.64f, 1.27f, 0.99f);
    vec3 skycolor = vec3(0.6f, 1.5f, 1.0f);

    vec3 bg = exp(uv.y - 2.0f) * vec3(0.4f, 1.6f, 1.0f);

    float halo=  clamp(dot(normalize(vec3(-ro.x, -ro.y, -ro.z)), rd), 0.0f, 1.0f);
    vec3 col = bg + vec3(1.0f, 0.8f, 0.4f) * pow(halo, 17.0f);

    float t = 0.0f;
    vec3 p = ro;

    vec3 res = intersect(ro, rd);
    if(res.x > 0.0f){
        p = ro + res.x * rd;
        vec3 n = nor(p);
        float shadow = softshadow(p, sundir, 10.0f);

        float dif = max(0.0f, dot(n, sundir));
        float sky = 0.6 + 0.4 * max(0.0, dot(n, vec3(0.0f, 1.0f, 0.0f)));
        float bac = max(0.3f + 0.7f * dot(vec3(-sundir.x, -1.0f, -sundir.z), n), 0.0f);
        float spe = max(0.0f, pow(clamp(dot(sundir, reflect(rd, n)), 0.0f, 1.0f), 10.0f));

        vec3 lin = 4.5f * sun * dif * shadow;
        lin += 0.8f * bac * sun;
        lin += 0.6f * sky * skycolor*shadow;
        lin += 3.0f * spe * shadow;

        res.y = pow(clamp(res.y, 0.0f, 1.0f), 0.55f);
        vec3 tc0 = 0.5f + 0.5f * sin(3.0f + 4.2f * res.y + vec3(0.0f, 0.5f, 1.0f));
        col = lin * vec3(0.9f, 0.8f, 0.6f) *  0.2f * tc0;
        col = mix(col, bg, 1.0f - exp(-0.001f * res.x * res.x));
    }

    // post
    col = pow(clamp(col, 0.0f, 1.0f), vec3(0.45f));
    col = col * 0.6f + 0.4f * col * col * (3.0f - 2.0f * col);  // contrast
    col = mix(col, vec3(dot(col, vec3(0.33))), -0.5f);  // satuation
    col *= 0.5f + 0.5f * pow(16.0f * q.x * q.y * (1.0f - q.x) * (1.0f - q.y), 0.7f);  // vigneting
    fragColor = vec4(col.xyz, smoothstep(0.55f, 0.76f, 1.0f - res.x / 5.0f));
}