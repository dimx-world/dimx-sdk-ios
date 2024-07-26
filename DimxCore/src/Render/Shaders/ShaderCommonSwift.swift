//
//  ShaderCommonSwift.swift
//  DimxCore
//
//  Created by Sergii Romanov on 24/07/2023.
//  Copyright © 2023 Dimensions. All rights reserved.
//

import Foundation

import simd

let METAL_MAX_MORPH_TARGETS_BLEND = 20

struct StandardVertexUniforms {
    var vViewMat: matrix_float4x4 = matrix_float4x4()
    var vViewProjMat: matrix_float4x4 = matrix_float4x4()
    var vModelMat: matrix_float4x4 = matrix_float4x4()
    var vNormalMat: matrix_float3x3 = matrix_float3x3()
    var vLightSpaceMat: matrix_float4x4 = matrix_float4x4()

    var vNumMeshVerts: CInt = 0
    var vMorphTargetInds: (CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt, CInt) = (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    var vMorphTargetWeights: (CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat, CFloat) = (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    var vMorphNumTargets: CInt = 0
    var vMorphVertComps: CInt = 0
}
