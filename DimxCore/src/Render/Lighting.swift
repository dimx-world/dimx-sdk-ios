//
//  Lighting.swift
//  dimx-ios-app
//
//  Created by Sergii Romanov on 12/07/2021.
//  Copyright © 2021 Dimensions. All rights reserved.
//

import Foundation
import Metal
import simd
import DimxNative
class Lighting
{
    let coreLighting: UnsafeRawPointer

    let enabled = true
   
    var volumeWidth = Float(1.0)
    var volumeHeight = Float(1.0)
    var volumeNear = Float(0.1)
    var volumeFar = Float(1.0)
    var ambientColor = vector_float3()
    var diffuseColor = vector_float3()
    var specularColor = vector_float3()
    var shadowEnabled = false
    var shadowMapSize = 0
    var shadowSoftness = Float(1.0)
    var shadowDarkness = Float(1.0)
    var shadowDecayDistScaled = Float(1.0)

    var position = vector_float3()
    var direction = vector_float3()
    var lightSpaceMat = matrix_float4x4()
    
    init(_ ptr: UnsafeRawPointer) {
        coreLighting = ptr

        volumeWidth = Lighting_volumeWidth(coreLighting)
        volumeHeight = Lighting_volumeHeight(coreLighting)
        volumeNear = Lighting_volumeNear(coreLighting)
        volumeFar = Lighting_volumeFar(coreLighting)

        Lighting_ambientColor(coreLighting, &ambientColor)
        Lighting_diffuseColor(coreLighting, &diffuseColor)
        Lighting_specularColor(coreLighting, &specularColor)

        shadowEnabled = Lighting_shadowEnabled(coreLighting)
        shadowMapSize = Lighting_shadowMapSize(coreLighting)
        shadowSoftness = Lighting_shadowSoftness(coreLighting)
        shadowDarkness = Lighting_shadowDarkness(coreLighting)
        shadowDecayDistScaled = Lighting_shadowDecayDistScaled(coreLighting)
        
        onFrameUpdate()
    }
    
    func onFrameUpdate() {
        Lighting_position(coreLighting, &position)
        Lighting_direction(coreLighting, &direction)
        Lighting_lightSpaceMat(coreLighting, &lightSpaceMat)
    }
}
