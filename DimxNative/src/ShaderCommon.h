//
//  DimxCore.h
//  DimxCore
//
//  Created by Sergii Romanov on 21/07/2023.
//  Copyright © 2023 Dimensions. All rights reserved.
//

#import <Foundation/Foundation.h>

//! Project version number for DimxCore.
FOUNDATION_EXPORT double DimxCoreVersionNumber;

//! Project version string for DimxCore.
FOUNDATION_EXPORT const unsigned char DimxCoreVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <DimxCore/PublicHeader.h>



//#import "Render/Shaders/ShaderCommon.h"

#ifndef ShaderCommon_h
#define ShaderCommon_h

#include <simd/simd.h>

#ifdef __METAL_VERSION__
#define NS_ENUM(_type, _name) enum _name : _type _name; enum _name : _type
#define NSInteger int // TODO: use 32-bit NSInteger.. which is not available
#else
#import <Foundation/Foundation.h>
#endif

#define METAL_MAX_MORPH_TARGETS_BLEND 20
/*
struct StandardVertexUniforms {
    matrix_float4x4 vViewMat;
    matrix_float4x4 vViewProjMat;
    matrix_float4x4 vModelMat;
    matrix_float3x3 vNormalMat;
    matrix_float4x4 vLightSpaceMat;

    int   vNumMeshVerts;
    int   vMorphTargetInds[METAL_MAX_MORPH_TARGETS_BLEND];
    float vMorphTargetWeights[METAL_MAX_MORPH_TARGETS_BLEND];
    int   vMorphNumTargets;
    int   vMorphVertComps;
};
*/

//matrix_float4x4 vJointTransforms[196];

struct StandardFragmentUniforms {
    vector_float4 fBaseColor;
    float fBaseColorFactor;
    float fMetalness;
    float fRoughness;
    
    vector_float4 fAddColor;
    vector_float4 fMultColor;
    
    vector_float3 fCameraPos;
    
    bool fFlat;
    bool fReceiveShadows;

    float fRadianceMaxLod;

    vector_float3 fLightDir;
    vector_float3 fLightAmbientColor;
    vector_float3 fLightDiffuseColor;
    vector_float3 fLightSpecularColor;
    
    vector_float2 fShadowMapSize;
    float fShadowSoftness;
    float fShadowDarkness;
    float fShadowDecayDistScaled;

    bool fUseDepthOcclusion;
    matrix_float3x3 fDepthMapUVTransform;
    float fDepthMapAspectRatio;
};

typedef NS_ENUM(NSInteger, VertexAttribute)
{
    vPosition = 0,
    vPosition2,
    vNormal,
    vTangent,
    vBitangent,
    vTexCoord,
    vColor,
    vColorUB,
    vJointIndex,
    vJointIndices4,
    vJointWeights4,
    vNone
};

typedef NS_ENUM(NSInteger, VertexBufferIndex)
{
    VBIVertexBuffer = 0,
    VBIUniforms,
    VBIJointTransforms,
    VBIMorphInds,
    VBIMorphVerts,
};

typedef NS_ENUM(NSInteger, FragmentBufferIndex)
{
    FBIUniforms = 0
};

typedef NS_ENUM(NSInteger, FragmentTextureIndex)
{
    FTIShadowMap = 0,
    FTIDepthMap,
    FTIIrradianceMap,
    FTIRadianceMap,
    FTIBaseColorMap,
    FTINormalMap,
    FTIMetalnessMap,
    FTIRoughnessMap,
};

typedef NS_ENUM(NSInteger, FunctionConstant)
{
    // VertexAttribute and FunctionConstant must match core VertexAttribType!
    // Vertex attribute constants
    FCPositionAttr = 0,
    FCPosition2Attr,
    FCNormalAttr,
    FCTangentAttr,
    FCBitangentAttr,
    FCTexCoordAttr,
    FCColorAttr,
    FCColorUBAttr,
    FCJointIndexAttr,
    FCJointIndices4Attr,
    FCJointWeights4Attr,
    FCNoneAttr,
    
    // Add other constants below
    FCOcclusionPass,
    FCShadowsPass,
    FCMorphEnabled,
    FCMorphNormals,
    FCMorphTangents,
    
    FCBaseColorMap,
    FCNormalMap,
    FCMetalnessMap,
    FCRoughnessMap
};

#endif
