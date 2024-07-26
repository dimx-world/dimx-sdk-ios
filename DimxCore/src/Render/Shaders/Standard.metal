#include <metal_stdlib>

#include "ShaderCommon.h"

using namespace metal;

constant bool VAPosition      [[function_constant(FCPositionAttr)]];
constant bool VAPosition2     [[function_constant(FCPosition2Attr)]];
constant bool VANormal        [[function_constant(FCNormalAttr)]];
constant bool VATangent       [[function_constant(FCTangentAttr)]];
constant bool VABitangent     [[function_constant(FCBitangentAttr)]];
constant bool VATexCoord      [[function_constant(FCTexCoordAttr)]];
constant bool VAColor         [[function_constant(FCColorAttr)]];
constant bool VAColorUB       [[function_constant(FCColorUBAttr)]];
constant bool VAJointIndex    [[function_constant(FCJointIndexAttr)]];
constant bool VAJointIndices4 [[function_constant(FCJointIndices4Attr)]];
constant bool VAJointWeights4 [[function_constant(FCJointWeights4Attr)]];

constant bool OcclusionPass   [[function_constant(FCOcclusionPass)]];
constant bool ShadowsPass     [[function_constant(FCShadowsPass)]];
constant bool MorphEnabled    [[function_constant(FCMorphEnabled)]];
constant bool MorphNormals    [[function_constant(FCMorphNormals)]];
constant bool MorphTangents   [[function_constant(FCMorphTangents)]];
constant bool JointTransformsConst = VAJointIndex || VAJointIndices4;

constant bool MPBaseColorMap   [[function_constant(FCBaseColorMap)]];
constant bool MPNormalMap      [[function_constant(FCNormalMap)]];
constant bool MPMetalnessMap   [[function_constant(FCMetalnessMap)]];
constant bool MPRoughnessMap   [[function_constant(FCRoughnessMap)]];

struct StandardVertexIn {
    float3 vPosition      [[attribute(VertexAttribute::vPosition),      function_constant(VAPosition)]];
    float2 vPosition2     [[attribute(VertexAttribute::vPosition2),     function_constant(VAPosition2)]];
    float3 vNormal        [[attribute(VertexAttribute::vNormal),        function_constant(VANormal)]];
    float3 vTangent       [[attribute(VertexAttribute::vTangent),       function_constant(VATangent)]];
    float3 vBitangent     [[attribute(VertexAttribute::vBitangent),     function_constant(VABitangent)]];
    float2 vTexCoord      [[attribute(VertexAttribute::vTexCoord),      function_constant(VATexCoord)]];
    float4 vColor         [[attribute(VertexAttribute::vColor),         function_constant(VAColor)]];
    uchar4 vColorUB       [[attribute(VertexAttribute::vColorUB),       function_constant(VAColorUB)]];
    float  vJointIndex    [[attribute(VertexAttribute::vJointIndex),    function_constant(VAJointIndex)]];
    // Opengl doesn't support vector of ints so we have to use floats in metal too
    float4 vJointIndices4 [[attribute(VertexAttribute::vJointIndices4), function_constant(VAJointIndices4)]];
    float4 vJointWeights4 [[attribute(VertexAttribute::vJointWeights4), function_constant(VAJointWeights4)]];
};

struct StandardVertexOut {
    float4 position  [[position]];
    float3 normal    [[function_constant(VANormal)]];
    float2 texCoords [[function_constant(VATexCoord)]];
    float4 color     [[function_constant(VAColor)]];
    float3 worldSpacePos;
    float4 lightSpacePos;
};

struct MorphDeform {
    float3 position{0, 0, 0};
    float3 normal{0, 0, 0};
    float3 tangent{0, 0, 0};
};

constant float PI = 3.14159265359;

float4x4 calcJointMatrix(float4 indices, float4 weights, constant const float4x4* jointTransforms)
{
    return jointTransforms[(int)indices[0]] * weights[0] +
           jointTransforms[(int)indices[1]] * weights[1] +
           jointTransforms[(int)indices[2]] * weights[2] +
           jointTransforms[(int)indices[3]] * weights[3];
}

float calcShadow(depth2d<float> shadowMap, float4 lightSpacePos, constant StandardFragmentUniforms& uniforms)
{
    constexpr float bias = 0.005;
    constexpr sampler shadowMapSampler(filter::nearest, address::clamp_to_zero, compare_func::less);
    // Kernel used:
    // 0   4   7   4   0
    // 4   16  26  16  4
    // 7   26  41  26  7
    // 4   16  26  16  4
    // 0   4   7   4   0
    struct SampleInfo {
        float weight;
        float2 shift;
    };
    const int numSamples = 21;
    const float totalWeights = 269.0;
    const SampleInfo samples[numSamples] = {
        {41.0, {0.0,  0.0}},
        {26.0, {1.0,  0.0}},
        {26.0, {1.0,  0.0}},
        {26.0, {0.0, -1.0}},
        {26.0, {0.0,  1.0}},
        {16.0, {1.0, -1.0}},
        {16.0, {1.0, -1.0}},
        {16.0, {1.0,  1.0}},
        {16.0, {1.0,  1.0}},
        { 7.0, {0.0, -2.0}},
        { 7.0, {2.0,  0.0}},
        { 7.0, {2.0,  0.0}},
        { 7.0, {0.0,  2.0}},
        { 4.0, {1.0, -2.0}},
        { 4.0, {1.0, -2.0}},
        { 4.0, {2.0, -1.0}},
        { 4.0, {2.0, -1.0}},
        { 4.0, {2.0,  1.0}},
        { 4.0, {2.0,  1.0}},
        { 4.0, {1.0,  2.0}},
        { 4.0, {1.0,  2.0}}
    };
    
    float3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords.x = projCoords.x * 0.5 + 0.5;
    projCoords.y = 1.0 - (projCoords.y * 0.5 + 0.5);

    float minLookupStep = 1.0 / uniforms.fShadowMapSize.x * uniforms.fShadowSoftness;
    float shadow = 0.0;
    float maxScore = 1.0;
    for (int i = 0; i < numSamples; ++i) {
        float shadeScore = shadow / maxScore;
        float stepFactor = 1.0 + (1.0 - shadeScore * shadeScore) * 10.0;
        float lookupStep = minLookupStep * stepFactor;
        float2 texCoords = clamp(projCoords.xy + samples[i].shift * lookupStep, 0.0, 1.0);
        float depthDist = projCoords.z - shadowMap.sample(shadowMapSampler, texCoords);
        float sampleShade = depthDist / uniforms.fShadowDecayDistScaled;
        if (depthDist > bias && sampleShade < 1.0) {
            maxScore += samples[i].weight;
            shadow += (1.0 - sampleShade * sampleShade) * samples[i].weight;
        }
    }
    shadow /= totalWeights;

    return shadow * uniforms.fShadowDarkness;
}

float4 calcLight(float3 faceNormal, constant StandardFragmentUniforms& uniforms)
{
    float diff = dot(normalize(faceNormal), -uniforms.fLightDir) * 0.5 + 0.5;
    float3 lightCol = uniforms.fLightAmbientColor + uniforms.fLightDiffuseColor * diff;
    return float4(clamp(lightCol, 0.f, 1.f), 1.f);
}

MorphDeform calcMorphDeform(constant StandardVertexUniforms& uniforms,
                            constant uint32_t* morphInds,
                            constant float* morphVerts,
                            unsigned int vid)
{
/*
     Note: we take morphVerts as float* instead of float3* (which would be easier to address)
           because metal aligns float3 as float4.
 */
    MorphDeform result;
    
    for (int i = 0; i < uniforms.vMorphNumTargets; ++i) {
        int indPos = uniforms.vMorphTargetInds[i] * uniforms.vNumMeshVerts + vid;
        int vertLoc = morphInds[indPos] * 3 * uniforms.vMorphVertComps;
        if (vertLoc > 0) {
            result.position += float3(morphVerts[vertLoc + 0],
                                      morphVerts[vertLoc + 1],
                                      morphVerts[vertLoc + 2]) * uniforms.vMorphTargetWeights[i];
            if (MorphNormals) {
                result.normal += float3(morphVerts[vertLoc + 3],
                                        morphVerts[vertLoc + 4],
                                        morphVerts[vertLoc + 5]) * uniforms.vMorphTargetWeights[i];
            }
            if (MorphTangents) {
                result.tangent += float3(morphVerts[vertLoc + 6],
                                         morphVerts[vertLoc + 7],
                                         morphVerts[vertLoc + 8]) * uniforms.vMorphTargetWeights[i];
            }
        }
    }

    return result;
}

vertex StandardVertexOut standard_vertex(StandardVertexIn in [[stage_in]],
                                         constant StandardVertexUniforms& uniforms [[buffer(VBIUniforms)]],
                                         constant float4x4* jointTransforms [[buffer(VBIJointTransforms), function_constant(JointTransformsConst)]],
                                         constant uint32_t* morphInds [[buffer(VBIMorphInds), function_constant(MorphEnabled)]],
                                         constant float* morphVerts [[buffer(VBIMorphVerts), function_constant(MorphEnabled)]],
                                         unsigned int vid [[vertex_id]])
{
    StandardVertexOut out;

    float3 position = in.vPosition;
    float3 normal = in.vNormal;
    float3 tangent = in.vTangent;

    if (MorphEnabled) {
        MorphDeform deform = calcMorphDeform(uniforms, morphInds, morphVerts, vid);
        position += deform.position;
        normal += deform.normal;
        tangent += deform.tangent;
    }
    
    float4x4 modelMat = uniforms.vModelMat;
    if (VAJointIndex) {
        modelMat = modelMat * jointTransforms[(int)in.vJointIndex];
    } else if (VAJointIndices4) {
        modelMat = modelMat * calcJointMatrix(in.vJointIndices4, in.vJointWeights4, jointTransforms);
    }

    float4 worldSpacePos = modelMat * float4(position, 1);
    out.position = uniforms.vViewProjMat * worldSpacePos;
    out.worldSpacePos = float3(worldSpacePos.x, worldSpacePos.y, worldSpacePos.z);

    if (VANormal) {
        //out.normal = normalize(uniforms.vNormalMat * normal);
        out.normal = normalize(float3(modelMat * float4(normal, 0)));
    }
    if (VATexCoord) {
        out.texCoords = in.vTexCoord;
    }
    if (VAColor) {
        out.color = in.vColor;
    }
    
    out.lightSpacePos = uniforms.vLightSpaceMat * worldSpacePos;
    
    return out;
}

//-----------------------------------------------------------------
float3 applyNormalMap(float3 N, StandardVertexOut in, texture2d<float, access::sample> normalMap)
{
    constexpr sampler linearSampler(address::repeat, filter::linear);
    // Sample the normal map and convert from [0, 1] to [-1, 1]
    float3 tangentNormal = normalMap.sample(linearSampler, in.texCoords).xyz * 2.0 - 1.0;

    // Calculate partial derivatives
    float3 Q1  = dfdx(in.worldSpacePos);
    float3 Q2  = dfdy(in.worldSpacePos);
    float2 st1 = dfdx(in.texCoords);
    float2 st2 = dfdy(in.texCoords);

    // Compute the TBN matrix
    float3 T  = normalize(Q1*st2.y - Q2*st1.y);
    float3 B  = -normalize(cross(N, T));
    float3x3 TBN = float3x3(T, B, N);

    // Transform the tangent normal to the world space
    return normalize(TBN * tangentNormal);
}
float DistributionGGX(float3 N, float3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness) {
    return F0 + (max(float3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float3 calcReflection(StandardVertexOut in, float3 albedo, float3 N, float3 V, float3 F0, float metalness, float roughness, float3 lightPos, float3 lightCol) {
    float3 L = normalize(lightPos - in.worldSpacePos);
    float3 H = normalize(V + L);
    float distance = length(lightPos - in.worldSpacePos);
    float attenuation = 1.0 / (distance * distance);
    float3 radiance = lightCol * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    float3 F  = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    float3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    float3 specular = numerator / denominator;

    // kS is equal to Fresnel
    float3 kS = F;
    // Diffuse component
    float3 kD = float3(1.0) - kS;
    kD *= 1.0 - metalness;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance
    return (kD * albedo / PI + specular) * radiance * NdotL;
}
//-----------------------------------------------------------------
fragment float4 standard_fragment(StandardVertexOut in [[stage_in]],
                                 constant StandardFragmentUniforms& uniforms [[buffer(FBIUniforms)]],
                                 depth2d<float> shadowMap [[texture(FTIShadowMap)]],
                                 texturecube<half, access::sample> irradianceMap [[texture(FTIIrradianceMap)]],
                                 texturecube<half, access::sample> radianceMap [[texture(FTIRadianceMap)]],
                                 texture2d<half, access::sample> baseColorMap [[texture(FTIBaseColorMap), function_constant(MPBaseColorMap)]],
                                 texture2d<float, access::sample> normalMap [[texture(FTINormalMap), function_constant(MPNormalMap)]],
                                 texture2d<half, access::sample> metalnessMap [[texture(FTIMetalnessMap), function_constant(MPMetalnessMap)]],
                                 texture2d<half, access::sample> roughnessMap [[texture(FTIRoughnessMap), function_constant(MPRoughnessMap)]])
{
    constexpr sampler linearSampler(address::repeat, filter::linear);
    constexpr sampler cubeSampler(address::clamp_to_edge, filter::linear, mip_filter::linear);
    
    if (OcclusionPass) {
        // In OpenGL we can disable color write. But in Metal we just write a fully transparent pixel. The depth write goes as normal.
        return float4(0, 0, 0, 0);
    }

    if (ShadowsPass) {
        return float4(0, 0, 0, calcShadow(shadowMap, in.lightSpacePos, uniforms));
    }
    
    float4 baseColor = uniforms.fBaseColor;

    if (VAColor) {
        baseColor *= in.color;
    }

    if (VATexCoord && MPBaseColorMap) {
        half4 texCol = baseColorMap.sample(linearSampler, in.texCoords);
        baseColor = float4(texCol) * (1.0 - uniforms.fBaseColorFactor) + baseColor * uniforms.fBaseColorFactor;
        baseColor.a = texCol.a;
    }
    
    if (uniforms.fFlat) {
        return baseColor;
    }

/*
    // This kills fps
    if (baseColor.a < 0.001) {
        discard_fragment();
    }
*/

    float3 N = float3(0, 1, 0);
    if (VANormal) {
        N = normalize(in.normal);
    }
    
    if (VATexCoord && MPNormalMap) {
        N = applyNormalMap(N, in, normalMap);
    }
    
    float3 V = normalize(uniforms.fCameraPos - in.worldSpacePos);
    float3 R = reflect(-V, N);

    float metallic = uniforms.fMetalness;
    if (VATexCoord && MPMetalnessMap) {
        metallic = metalnessMap.sample(linearSampler, in.texCoords).r;
    }

    float roughness = uniforms.fRoughness;
    if (VATexCoord && MPRoughnessMap) {
        roughness = roughnessMap.sample(linearSampler, in.texCoords).r;
    }

    float3 F0 = float3(0.04);
    F0 = mix(F0, float3(baseColor), metallic);

    // Loop per light
    float3 Lo = calcReflection(in, float3(baseColor), N, V, F0, metallic, roughness, float3(0, 5, 1), float3(1, 1, 1));

    // Ambient lighting using IBL
    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    float3 kS = F;
    float3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    float3 irradiance = float3(irradianceMap.sample(cubeSampler, N).rgb);
    float3 diffuse = irradiance * float3(baseColor);

    // IBL specular part
    float3 prefilteredColor = float3(radianceMap.sample(cubeSampler, R, level(roughness * uniforms.fRadianceMaxLod)).rgb);
    float3 specular = prefilteredColor * F;

    float3 ambient = kD * diffuse + specular;

    float3 color = ambient + Lo;

    baseColor.xyz = color;
    
    //baseColor.xyz = float3(specularLod);
    
    float4 outColor = (baseColor + uniforms.fAddColor) * uniforms.fMultColor;
    
    if (uniforms.fReceiveShadows) {
        outColor.xyz *= 1.0 - calcShadow(shadowMap, in.lightSpacePos, uniforms);
    }
    
    return outColor;
}
