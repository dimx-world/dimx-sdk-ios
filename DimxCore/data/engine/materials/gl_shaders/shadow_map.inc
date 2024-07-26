#include "lighting.inc"

uniform sampler2D fShadowMap;
uniform vec2 fShadowMapSize;
uniform float fShadowSoftness;
uniform float fShadowDarkness;
uniform float fShadowDecayDistScaled;

//vec4 fLightVolume: width, height, near, far

struct ShadowSampleInfo {
    float weight;
    vec2 offset;
};

float calcShadow(vec4 lightSpacePos)
{
  // Kernel used:
  // 0   4   7   4   0
  // 4   16  26  16  4
  // 7   26  41  26  7
  // 4   16  26  16  4
  // 0   4   7   4   0

    const int numSamples = 21;
    const float totalWeights = 269.0;
    const ShadowSampleInfo samples[numSamples] = ShadowSampleInfo[](
        ShadowSampleInfo(41.0, vec2( 0.0,  0.0)),
        ShadowSampleInfo(26.0, vec2(-1.0,  0.0)),
        ShadowSampleInfo(26.0, vec2( 1.0,  0.0)),
        ShadowSampleInfo(26.0, vec2( 0.0, -1.0)),
        ShadowSampleInfo(26.0, vec2( 0.0,  1.0)),
        ShadowSampleInfo(16.0, vec2(-1.0, -1.0)),
        ShadowSampleInfo(16.0, vec2( 1.0, -1.0)),
        ShadowSampleInfo(16.0, vec2(-1.0,  1.0)),
        ShadowSampleInfo(16.0, vec2( 1.0,  1.0)),
        ShadowSampleInfo( 7.0, vec2( 0.0, -2.0)),
        ShadowSampleInfo( 7.0, vec2(-2.0,  0.0)),
        ShadowSampleInfo( 7.0, vec2( 2.0,  0.0)),
        ShadowSampleInfo( 7.0, vec2( 0.0,  2.0)),
        ShadowSampleInfo( 4.0, vec2(-1.0, -2.0)),
        ShadowSampleInfo( 4.0, vec2( 1.0, -2.0)),
        ShadowSampleInfo( 4.0, vec2(-2.0, -1.0)),
        ShadowSampleInfo( 4.0, vec2( 2.0, -1.0)),
        ShadowSampleInfo( 4.0, vec2(-2.0,  1.0)),
        ShadowSampleInfo( 4.0, vec2( 2.0,  1.0)),
        ShadowSampleInfo( 4.0, vec2(-1.0,  2.0)),
        ShadowSampleInfo( 4.0, vec2( 1.0,  2.0))
    );

    float bias = 0.0005;
#ifdef VA_vNormal
    bias += 0.0005 * (1.0 - abs(dot(normalize(fNormal), -fLightDir)));
#endif
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w; // perform perspective divide
    projCoords = projCoords * 0.5 + 0.5;                   // transform to [0,1] range

    float minLookupStep = 1.0 / fShadowMapSize.x * fShadowSoftness;
    float shadow = 0.0;
    float maxScore = 1.0;
    for (int i = 0; i < numSamples; ++i) {
        float shadeScore = shadow / maxScore;
        float stepFactor = 1.0 + (1.0 - shadeScore * shadeScore) * 10.0;
        float lookupStep = minLookupStep * stepFactor;
        ShadowSampleInfo info = samples[i]; // samples[i].shift fails to compile on Xiaomi
        vec2 texCoords = clamp(projCoords.xy + info.offset * lookupStep, 0.0, 1.0);
        float depthDist = projCoords.z - texture(fShadowMap, texCoords).r;
        float sampleShade = depthDist / fShadowDecayDistScaled;
        if (depthDist > bias && sampleShade < 1.0) {
            maxScore += info.weight;
            shadow += (1.0 - sampleShade * sampleShade) * info.weight;
        }
    }
    shadow /= totalWeights;

    return shadow * fShadowDarkness;
}

float calcShadow_old(vec4 lightSpacePos)
{
    float bias = 0.0005;
#ifdef VA_vNormal
    bias += 0.0005 * (1.0 - abs(dot(normalize(fNormal), -fLightDir)));
#endif
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w; // perform perspective divide
    projCoords = projCoords * 0.5 + 0.5;                   // transform to [0,1] range

    float lookupStep = 1.0 / 1024.0 * fShadowSoftness;
    float shadow = 0.0;
    for(int dx = -2; dx <= 2; ++dx) {
        for(int dy = -2; dy <= 2; ++dy) {
            vec2 texShift = vec2(float(dx) * lookupStep, float(dy) * lookupStep);
            shadow += projCoords.z - texture(fShadowMap, projCoords.xy + texShift).r > bias  ? 1.0 : 0.0;
        }
    }
    
    return (shadow / 25.0) * fShadowDarkness;
}
