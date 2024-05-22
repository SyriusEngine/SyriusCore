#version 460

layout (location = 16) in vec4 fColor;
layout (location = 17) in vec3 fNormal;
layout (location = 18) in vec3 fTangent;
layout (location = 19) in vec2 fTexCoord;

layout (location = 0) out vec4 fragColor;

layout (std140, binding = 5) uniform Parameters {
    vec2 resolution;
    float time;
    float deltaTime;
    vec4 mouse;
};

#define AA  0

float PrRoundBoxDf(vec3 p, vec3 b, float r);
float SmoothBump(float lo, float hi, float w, float x);
vec2 Rot2D(vec2 q, float a);
float Hashfv2(vec2 p);
vec3 VaryNf(vec3 p, vec3 n, float f);

vec3 ballPos, ltDir;
float dstFar, frctAng;
int idObj;
const float pi = 3.14159265359f;

float ObjDf(vec3 p) {
    float dMin = dstFar;
    float d = 0.47f - abs(p.y - 3.5f);
    if (d < dMin) {
        dMin = d;
        idObj = 1;
    }
    p.xz = abs(0.5f - mod((2.0f / 3.0f) * p.xz, 1.0f));
    float s = 1.0f;
    for (int j = 0; j < 9; j++) {
        p = abs(p) - vec3(-0.02f, 1.98f, -0.02f);
        float f = 2.0f / clamp(dot(p, p), 0.4f, 1.0f);
        p = f * p - vec3(0.5f, 1.0f, 0.4f);
        s *= f;
        p.xz = Rot2D(p.xz, frctAng);
    }
    d = PrRoundBoxDf(p, vec3(0.1f, 5.0f, 0.1f), 0.1f) / s;
    if (d < dMin) {
        dMin = d;
        idObj = 2;
    }
    return dMin;
}

vec3 ObjNf(vec3 p) {
    vec2 e = vec2(0.0001f, -0.0001f);
    vec4 v = vec4(ObjDf(p + e.xxx), ObjDf(p + e.xyy), ObjDf(p + e.yxy), ObjDf(p + e.yyx));
    return normalize(vec3(v.x - v.y - v.z - v.w) + 2.0f * v.yzw);
}

float ObjAO(vec3 ro, vec3 rd) {
    float ao = 0.0f;
    for (float j = 1.0f; j < 5.; j++) {
        float d = 0.002 * j * j;
        ao += max(0.0f, d - ObjDf(ro + rd * d));
    }
    return 0.2f + 0.8f * clamp(1.0f - 20.0f * ao, 0.0f, 1.0f);
}

float BallHit(vec3 ro, vec3 rd) {
    float bRad = 0.025f;
    float b = dot(rd, ro);
    float d = b * b + bRad * bRad - dot(ro, ro);
    if (d > 0.0f) {
        d = -b - sqrt(d);
        if (d < 0.0f) {
            d = dstFar;
        }
    }
    else{
        d = dstFar;
    }
    return d;
}

float TxPattern(vec3 p) {
    p = abs(0.5f - fract(4. * p));
    float c = 0.0f;
    float t = 0.0f;
    for (float j = 0.; j < 6.; j++) {
        p = abs(p + 3.0f) - abs(p - 3.0f) - p;
        p /= clamp(dot(p, p), 0.0f, 1.0f);
        p = 3.0f - 1.5f * p;
        if (mod(j, 2.0f) == 0.0f) {
            float tt = t;
            t = length(p);
            c += exp(-1.0f / abs(t - tt));
        }
    }
    return c;
}

vec3 ShowScene(vec3 ro, vec3 rd) {
    vec3 vn, rg, col, bgCol, amb;
    float dstBall, dstObj, dstGlow, d, eGap, bGlow, eGlow, eFlash, eMid, eVib, rnd;
    int idObjT;
    bool isRefl;
    frctAng = 0.18172 + pi * (2. * SmoothBump(0.25, 0.75, 0.25, mod(0.01 * time, 1.)) - 1.);
    dstBall = BallHit(ro - ballPos, rd);
    isRefl = false;
    if (dstBall < dstFar) {
        isRefl = true;
        ro += dstBall * rd;
        rd = reflect(rd, normalize(ro - ballPos));
    }
    dstObj = 0.;
    for (int j = 0; j < 150; j++) {
        d = ObjDf(ro + dstObj * rd);
        dstObj += d;
        if (d > 0.03) dstGlow = dstObj;
        if (d < 0.001 || dstObj > dstFar) break;
    }
    bgCol = vec3(0.9, 0.9, 1.) * (0.1 + 0.1 * max(0., dot(rd, normalize(vec3(0.5, 0.2, 1.)))));
    if (dstObj < dstFar) {
        if (idObj == 2) {
            rnd = Hashfv2(11. + 77. * floor((ro + dstObj * rd).xz / 1.5));
            bGlow = 0.;
            eGlow = 0.;
            eGap = mod(0.0625 * (time + 7. * rnd), 1.);
            eMid = 3.35;
            eVib = eMid + 0.005 * rnd * sin((64. + rnd) * time);
            for (int j = 0; j < 30; j++) {
                rg = ro + dstGlow * rd;
                d = ObjDf(rg);
                dstGlow += d;
                bGlow += max(0., 0.02 - d) * exp(-0.5 * dstGlow);
                eGlow += max(0., 0.03 - d) * (pow(max(0., 1. - 7. * abs(eVib - rg.y)), 8.) +
                pow(max(0., 1. - 20. * min(abs(eMid - rg.y - eGap), abs(eMid - rg.y + eGap))), 4.));
                if (d < 0.001 || dstGlow > dstFar) break;
            }
            eFlash = 0.6 + 0.4 * sin(8. * (time + 7. * rnd + rg.z));
        }
        ro += dstObj * rd;
        idObjT = idObj;
        vn = ObjNf(ro);
        amb = vec3(0.2);
        if (idObjT == 1) {
            col = vec3(0.1) * (1. + TxPattern((ro.y < 3.5) ? ro : 2. * ro));
            if (ro.y < 3.2) col *= vec3(0.9, 1., 0.9);
            vn = VaryNf(128. * ro, vn, 0.2);
        } else if (idObjT == 2) {
            col = vec3(0.7, 0.7, 0.6);
            amb += 1.5 * eFlash * vec3(1., 0.2, 0.1) * pow(max(0., 1. - 2.5 * abs(eMid - ro.y)), 2.) *
            max(0.5, sign(eMid - ro.y) * vn.y);
        }
        col = ObjAO(ro, vn) * col * (amb + max(0., dot(ltDir, vn)) +
        pow(max(dot(normalize(ltDir - rd), vn), 0.), 8.));
        col = mix(col, bgCol, 1. - exp(-dstObj));
        if (idObjT == 2) col += mix(vec3(0., 0.3, 1.), vec3(0., 1., 0.3), rnd) *
        (bGlow + 20. * eGlow * eGlow * eFlash) * (1. - smoothstep(0.4, 1., dstObj / dstFar));
    } else col = bgCol;
    if (isRefl) col = mix(mix(0.9 * col, vec3(0.7, 0.7, 0.8), 0.2), bgCol, 1. - exp(-dstBall));
    return col;
}

vec3 TrackPath(float t) {
    return vec3(0.75f * sin(t), 3.35f + 0.15f * sin(0.8f * t), 0.75f * cos(0.5f * t));
}

float PrRoundBoxDf(vec3 p, vec3 b, float r) {
    return length(max(abs(p) - b, 0.0f)) - r;
}

float SmoothBump(float lo, float hi, float w, float x) {
    return (1.0f - smoothstep(hi - w, hi + w, x)) * smoothstep(lo - w, lo + w, x);
}

vec2 Rot2D(vec2 q, float a) {
    vec2 cs = sin(a + vec2(0.5f * pi, 0.));
    return vec2(dot(q, vec2(cs.x, - cs.y)), dot(q.yx, cs));
}

const float cHashM = 43758.54f;

float Hashfv2(vec2 p) {
    return fract(sin(dot(p, vec2(37.0f, 39.0f))) * cHashM);
}

vec2 Hashv2v2(vec2 p) {
    vec2 cHashVA2 = vec2(37.0f, 39.0f);
    return fract(sin(vec2(dot(p, cHashVA2), dot(p + vec2(1.0f, 0.0f), cHashVA2))) * cHashM);
}

float Noisefv2(vec2 p) {
    vec2 ip = floor(p);
    vec2 fp = fract(p);
    fp = fp * fp * (3.0f - 2.0f * fp);
    vec2 t = mix(Hashv2v2(ip), Hashv2v2(ip + vec2(0.0f, 1.0f)), fp.y);
    return mix(t.x, t.y, fp.x);
}

float Fbmn(vec3 p, vec3 n) {
    vec3 s = vec3(0.0f);
    float a = 1.0f;
    for (int j = 0; j < 5; j++) {
        s += a * vec3(Noisefv2(p.yz), Noisefv2(p.zx), Noisefv2(p.xy));
        a *= 0.5f;
        p *= 2.0f;
    }
    return dot(s, abs(n));
}

vec3 VaryNf(vec3 p, vec3 n, float f) {
    vec2 e = vec2(0.1f, 0.0f);
    vec3 g = vec3(Fbmn(p + e.xyy, n), Fbmn(p + e.yxy, n), Fbmn(p + e.yyx, n)) - Fbmn(p, n);
    return normalize(n + f * (g - n * dot(n, g)));
}

void main() {
    vec2 uv = 2. * gl_FragCoord.xy / resolution - 1.0f;
    uv.x *= resolution.x / resolution.y;
    vec4 mPtr = mouse;
    mPtr.xy = mPtr.xy / resolution - 0.5f;
    float az = 0.0f;
    float el = 0.02f * pi;
    if (mPtr.z > 0.0f) {
        az += 2.0f * pi * mPtr.x;
        el += 0.7f * pi * mPtr.y;
    }
    float t = 0.1f * time;
    ballPos = TrackPath(t + 0.4f);
    vec3 ro = TrackPath(t);
    vec3 vd = normalize(ballPos - ro);
    az += 1.1f * (0.5f * pi + atan(- vd.z, vd.x));
    el += 0.8f * asin(vd.y);
    el = clamp(el, -0.25f * pi, 0.25f * pi);
    vec2 ori = vec2(el, az);
    vec2 ca = cos(ori);
    vec2 sa = sin(ori);
    mat3 vuMat = mat3(ca.y, 0.0f, - sa.y, 0.0f, 1.0f, 0.0f, sa.y, 0.0f, ca.y) *
    mat3(1.0f, 0.0f, 0.0f, 0.0f, ca.x, - sa.x, 0.0f, sa.x, ca.x);
    dstFar = 5.0f;
    ltDir = normalize(vec3(1.0f, 1.5f, -1.0f));
    #if ! AA
    const float naa = 1.0f;
    #else
    const float naa = 4.0f;
    #endif
    vec3 col = vec3(0.0f);
    for (float a = 0.0f; a < naa; a++) {
        vec3 rd = vuMat * normalize(vec3(uv + step(1.5, naa) * Rot2D(vec2(0.71f / resolution.y, 0.0f),
        0.5 * pi * (a + 0.5f)), 1.2f));
        col += (1. / naa) * ShowScene(ro, rd);
    }
    fragColor = vec4(pow(clamp(col, 0.0f, 1.0f), vec3(0.9f)), 1.0f);
}