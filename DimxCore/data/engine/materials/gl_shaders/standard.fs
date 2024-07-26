DIMX_FLOAT_PRECISION

in vec4 fPosition;
in vec4 fWorldPosition;
in vec4 fViewPosition;
in vec4 fScreenSpacePos;
in vec4 fLightSpacePos;

#ifdef VA_vNormal
in vec3 fNormal;
#endif

#ifdef VA_vColor
in vec4 fVertColor;
#endif

#ifdef VA_vTexCoord
in vec2 fTexCoord;
#endif

out vec4 outColor;

uniform vec4 fBaseColor;
#if defined(VA_vTexCoord) && defined(MP_fBaseColorMap)
uniform sampler2D fBaseColorMap;
uniform float fBaseColorFactor;
#endif

#if defined(VA_vTexCoord) && defined(MP_fNormalMap)
uniform sampler2D fNormalMap;
#endif

uniform float fMetalness;
#if defined(VA_vTexCoord) && defined(MP_fMetalnessMap)
uniform sampler2D fMetalnessMap;
#endif

uniform float fRoughness;
#if defined(VA_vTexCoord) && defined(MP_fRoughnessMap)
uniform sampler2D fRoughnessMap;
#endif

uniform vec4 fAddColor;
uniform vec4 fMultColor;

uniform vec3 fCameraPos;

uniform bool fFlat;
uniform bool fReceiveShadows;

uniform samplerCube fIrradianceMap;
uniform samplerCube fRadianceMap;
uniform float fRadianceMaxLod;

#include "lighting.inc"
#include "shadow_map.inc"

#ifdef USE_DEPTH_FOR_OCCLUSION
#include "depth_occlusion.inc"
#endif

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anyways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
#if defined(VA_vTexCoord) && defined(MP_fNormalMap)
vec3 applyNormalMap(vec3 N)
{
    vec3 tangentNormal = texture(fNormalMap, fTexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fWorldPosition.xyz);
    vec3 Q2  = dFdy(fWorldPosition.xyz);
    vec2 st1 = dFdx(fTexCoord);
    vec2 st2 = dFdy(fTexCoord);

    //vec3 N   = normalize(fNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
#endif
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   
// ----------------------------------------------------------------------------
vec3 calcReflection(vec3 albedo, vec3 N, vec3 V, vec3 F0, float metalness, float roughness, vec3 lightPos, vec3 lightCol)
{
    vec3 L = normalize(lightPos - fWorldPosition.xyz);
    vec3 H = normalize(V + L);
    float distance = length(lightPos - fWorldPosition.xyz);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lightCol * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
       
    vec3 numerator    = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;
    
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metalness;     

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);        

    // add to outgoing radiance Lo
    return (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}

void main()
{
    vec4 baseColor = fBaseColor;

#ifdef VA_vColor
    baseColor *= fVertColor;
#endif

#if defined(VA_vTexCoord) && defined(MP_fBaseColorMap)
    vec4 texCol = texture(fBaseColorMap, fTexCoord);
    baseColor = texCol * (1.0 - fBaseColorFactor) + baseColor * fBaseColorFactor;
    baseColor.a = texCol.a;
#endif

    if (baseColor.a < 0.001) {
        discard;
    }

    if (fFlat) {
        outColor = baseColor;
        return;
    }


#ifdef VA_vNormal
    vec3 N = normalize(fNormal);
#else
    vec3 N = vec3(0, 1, 0);
#endif

#if defined(VA_vTexCoord) && defined(MP_fNormalMap)
    N = applyNormalMap(N);
#endif

    vec3 V = normalize(fCameraPos - fWorldPosition.xyz);
    vec3 R = reflect(-V, N); 

#if defined(VA_vTexCoord) && defined(MP_fMetalnessMap)
    float metallic = texture(fMetalnessMap, fTexCoord).r;
#else
    float metallic = fMetalness;
#endif
#if defined(VA_vTexCoord) && defined(MP_fRoughnessMap)
    float roughness = texture(fRoughnessMap, fTexCoord).r;
#else
    float roughness = fRoughness;
#endif

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, baseColor.xyz, metallic);

    // loop per light
    vec3 Lo = calcReflection(baseColor.xyz, N, V, F0, metallic, roughness, vec3(0, 5, 1), vec3(1, 1, 1));

    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;     

    vec3 irradiance = texture(fIrradianceMap, N).rgb;
    vec3 diffuse    = irradiance * baseColor.xyz;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    vec3 prefilteredColor = textureLod(fRadianceMap, R,  roughness * fRadianceMaxLod).rgb;
    vec3 specular = prefilteredColor * F;

    //vec3 ambient = vec3(0.03) * baseColor.xyz;
    vec3 ambient = kD * diffuse + specular;

    vec3 color = ambient + Lo;

/*
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 
*/

    baseColor.xyz = color;

    outColor = (baseColor + fAddColor) * fMultColor;

#ifdef VA_vNormal
    //outColor.xyz *= calcLight(fNormal);
#endif

/*
// Commented out because some androids (Galaxy S20 FE 5G)
// every branch is actually executed regardles of the condition.
// Then the result is picked based on the condition.
// This leads to poor performance even when shadows are off.

    if (fReceiveShadows) {
        outColor.xyz *= 1.0 - calcShadow(fLightSpacePos);
    }
*/

#ifdef USE_DEPTH_FOR_OCCLUSION
    if (fUseDepthOcclusion) {
        outColor *= calcOcclusion(fViewPosition, fScreenSpacePos);
    }
#endif

}
