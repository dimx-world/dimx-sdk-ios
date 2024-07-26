import Foundation
import Metal
import simd
import DimxNative
class Material
{
    var defaultPipelineState: MTLRenderPipelineState!
    var occlusionPipelineState: MTLRenderPipelineState!
    var shadowMapPipelineState: MTLRenderPipelineState!
    var shadowsPipelineState: MTLRenderPipelineState!

    var vertUniforms: StandardVertexUniforms = StandardVertexUniforms()
    var numSkelJoints = 0
    var jointTransformsBuffer: MTLBuffer!
    var fragUniforms: StandardFragmentUniforms = StandardFragmentUniforms()
    
    var baseColor = vector_float4(0, 0, 0, 1)
    var baseColorFactor = Float(0.0)
    var baseColorMap: Texture?
    var normalMap: Texture?
    
    var flat = false
    
    var metalness = Float(0.0)
    var metalnessMap: Texture?

    var roughness = Float(1.0)
    var roughnessMap: Texture?
    
    var addColor = vector_float4(0, 0, 0, 0)
    var multColor = vector_float4(1, 1, 1, 1)
    
    var highlightFactor = Float(0.0)

    var transparent = false
    var numMeshVerts = Int32(0)
    var morphEnabled = false
    var morphNumVertComps = Int32(0)
    
    var stencilMode = StencilMode.none
    var stencilRefValue = UInt32(0)

    static func initCallbacks() {
        g_swiftMaterial().pointee.createMaterial = { (coreMat: Optional<UnsafeRawPointer>) -> (Int) in return Material.create(coreMat!) }
        g_swiftMaterial().pointee.deleteMaterial = { (id: Int) -> () in Material.delete(id) }

        g_swiftMaterial().pointee.setTextureParam = {
            (id: Int, paramName: UnsafePointer<CChar>!, coreTex: Optional<UnsafeRawPointer>) -> () in
                Material.setTextureParam(id, String(cString: paramName), coreTex!)
        }

        g_swiftMaterial().pointee.setBoolParam = {
            (id: Int, paramName: UnsafePointer<CChar>!, value: Bool) -> () in
                Material.setBoolParam(id, String(cString: paramName), value)
        }
        
        g_swiftMaterial().pointee.setFloatParam = {
            (id: Int, paramName: UnsafePointer<CChar>!, value: Float) -> () in
                Material.setFloatParam(id, String(cString: paramName), value)
        }

        g_swiftMaterial().pointee.setVec4Param = {
            (id: Int, paramName: UnsafePointer<CChar>!, x: Float, y: Float, z: Float, w: Float) -> () in
                Material.setVec4Param(id, String(cString: paramName), vector_float4(x, y, z, w))
        }
    }

    static func create(_ coreMat: UnsafeRawPointer) -> Int {
        Renderer.instance.materials.append(Material())
        return Renderer.instance.materials.count - 1
    }

    static func delete(_ id: Int) {
        Renderer.instance.materials[id] = nil
    }
    
    static func setTextureParam(_ id: Int, _ paramName: String, _ coreTex: UnsafeRawPointer) {
        let material = Renderer.instance.materials[id]!
        let texture = Renderer.instance.textures[Texture_nativeId(coreTex)]
        if paramName == "base_color_map" {
            material.baseColorMap = texture
        } else if paramName == "normal_map" {
            material.normalMap = texture
        } else if paramName == "metalness_map" {
            material.metalnessMap = texture
        } else if paramName == "roughness_map" {
            material.roughnessMap = texture
        } else {
            fatalError("Unknown texture param name to update: [\(paramName)]")
        }
    }

    static func setBoolParam(_ id: Int, _ paramName: String, _ value: Bool) {
        let material = Renderer.instance.materials[id]!
        if paramName == "flat" {
            material.flat = value
        } else {
            fatalError("Unknown bool param name to update: [\(paramName)]")
        }
    }
    
    static func setFloatParam(_ id: Int, _ paramName: String, _ value: Float) {
        let material = Renderer.instance.materials[id]!
        if paramName == "base_color_factor" {
            material.baseColorFactor = value
        } else if paramName == "metalness" {
            material.metalness = value
        } else if paramName == "roughness" {
            material.roughness = value
        } else {
            fatalError("Unknown float param name to update: [\(paramName)]")
        }
    }
    
    static func setVec4Param(_ id: Int, _ paramName: String, _ value: vector_float4) {
        let material = Renderer.instance.materials[id]!
        if paramName == "base_color" {
            material.baseColor = value
        } else if paramName == "add_color" {
            material.addColor = value
        } else if paramName == "mult_color" {
            material.multColor = value
        } else {
            fatalError("Unknown vec4 param name to update: [\(paramName)]")
        }
    }
    
    func initData(_ coreMat: UnsafeRawPointer, _ mesh: Mesh, _ numSkelJoints: Int) {
        transparent = Material_transparent(coreMat)

        Material_getParamVec4(coreMat, "base_color", &baseColor)
        baseColorFactor = Material_getParamFloat(coreMat, "base_color_factor")
        
        let baseColTexPtr = Material_getTexture(coreMat, "base_color_map")
        if baseColTexPtr != nil {
            baseColorMap = Renderer.instance.textures[Texture_nativeId(baseColTexPtr!)]
        }
        
        let normalTexPtr = Material_getTexture(coreMat, "normal_map")
        if normalTexPtr != nil {
            normalMap = Renderer.instance.textures[Texture_nativeId(normalTexPtr!)]
        }

        metalness = Material_getParamFloat(coreMat, "metalness")
        let metalnessTexPtr = Material_getTexture(coreMat, "metalness_map")
        if metalnessTexPtr != nil {
            metalnessMap = Renderer.instance.textures[Texture_nativeId(metalnessTexPtr!)]
        }

        roughness = Material_getParamFloat(coreMat, "roughness")
        let roughnessTexPtr = Material_getTexture(coreMat, "roughness_map")
        if roughnessTexPtr != nil {
            roughnessMap = Renderer.instance.textures[Texture_nativeId(roughnessTexPtr!)]
        }

        flat = Material_getParamBool(coreMat, "flat")
        
        let funcConsts = MTLFunctionConstantValues()
        populateVertAttribsConstants(funcConsts, mesh)

        var occlusonFlag = false
        var shadowsFlag = false
        funcConsts.setConstantValue(&occlusonFlag, type: MTLDataType.bool, index: FunctionConstant.FCOcclusionPass.rawValue)
        funcConsts.setConstantValue(&shadowsFlag, type: MTLDataType.bool, index: FunctionConstant.FCShadowsPass.rawValue)
        defaultPipelineState = try! Renderer.instance.device.makeRenderPipelineState(descriptor: createPipelineStateDescr(funcConsts, mesh))
        
        occlusonFlag = true
        funcConsts.setConstantValue(&occlusonFlag, type: MTLDataType.bool, index: FunctionConstant.FCOcclusionPass.rawValue)
        occlusionPipelineState = try! Renderer.instance.device.makeRenderPipelineState(descriptor: createPipelineStateDescr(funcConsts, mesh))

        occlusonFlag = false
        shadowsFlag = true
        funcConsts.setConstantValue(&occlusonFlag, type: MTLDataType.bool, index: FunctionConstant.FCOcclusionPass.rawValue)
        funcConsts.setConstantValue(&shadowsFlag, type: MTLDataType.bool, index: FunctionConstant.FCShadowsPass.rawValue)
        shadowsPipelineState = try! Renderer.instance.device.makeRenderPipelineState(descriptor: createPipelineStateDescr(funcConsts, mesh))
        
        shadowMapPipelineState = try! Renderer.instance.device.makeRenderPipelineState(descriptor: createShadowMapPipelineStateDescr(funcConsts, mesh))
        
        self.numSkelJoints = numSkelJoints
        if self.numSkelJoints > 0 {
            jointTransformsBuffer = Renderer.instance.device.makeBuffer(length: MemoryLayout<simd_float4x4>.stride * self.numSkelJoints, options: [])
        }
        
        numMeshVerts = Int32(mesh.numVerts)
        morphEnabled = mesh.morphEnabled
        morphNumVertComps = Int32(mesh.morphNumVertComps)
        
        stencilMode = stencilModeFromCore(Material_stencilMode(coreMat))
        stencilRefValue = UInt32(Material_stencilRefValue(coreMat))
    }
    
    func setupRender(_ renderer: Renderer,
                     _ encoder: MTLRenderCommandEncoder,
                     _ mesh: RenderableMesh,
                     _ frameContext: FrameContext,
                     occlusionPass: Bool,
                     shadowMapPass: Bool,
                     shadowsPass: Bool)
    {
        let renderable = mesh.getParentRenderable()
        let scene = renderable.getScene()
        
        vertUniforms.vViewMat = frameContext.viewMat
        vertUniforms.vViewProjMat = frameContext.viewProjectionMat
        vertUniforms.vLightSpaceMat = scene.lighting.lightSpaceMat
       
        vertUniforms.vModelMat = renderable.nodeTransform()
        vertUniforms.vNormalMat = renderable.nodeNormalTransform()

        if morphEnabled {
            vertUniforms.vNumMeshVerts = numMeshVerts
            vertUniforms.vMorphVertComps = morphNumVertComps
            vertUniforms.vMorphNumTargets = renderable.morphNumTargets()
            if vertUniforms.vMorphNumTargets > 0 {
                // If memcpy fails to build in release use the UnsafeMutableRawBufferPointer version below

                memcpy(&vertUniforms.vMorphTargetInds, renderable.morphTargetIds(), MemoryLayout<Int32>.size * Int(vertUniforms.vMorphNumTargets))
                //let indsDest = UnsafeMutableRawBufferPointer(start: &vertUniforms.vMorphTargetInds, count: Int(METAL_MAX_MORPH_TARGETS_BLEND) * MemoryLayout<Int32>.size)
                //let indsSrc = UnsafeRawBufferPointer(start: renderable.morphTargetIds(), count: Int(vertUniforms.vMorphNumTargets) * MemoryLayout<Int32>.size)
                //indsDest.copyMemory(from: indsSrc)

                memcpy(&vertUniforms.vMorphTargetWeights, renderable.morphTargetWeights(), MemoryLayout<Float>.size * Int(vertUniforms.vMorphNumTargets))
                //let weightsDest = UnsafeMutableRawBufferPointer(start: &vertUniforms.vMorphTargetWeights, count: Int(METAL_MAX_MORPH_TARGETS_BLEND) * MemoryLayout<Float>.size)
                //let weightsSrc = UnsafeRawBufferPointer(start: renderable.morphTargetWeights(), count: Int(vertUniforms.vMorphNumTargets) * MemoryLayout<Float>.size)
                //weightsDest.copyMemory(from: weightsSrc)
            }
        }

        fragUniforms.fBaseColor = baseColor
        fragUniforms.fBaseColorFactor = baseColorFactor
        fragUniforms.fMetalness = metalness
        fragUniforms.fRoughness = roughness
        
        fragUniforms.fFlat = flat
        
        let af = highlightFactor
        let mf = highlightFactor + 1.0
        fragUniforms.fAddColor = addColor + vector_float4(af, af, af, 0)
        fragUniforms.fMultColor = multColor * vector_float4(mf, mf, mf, 1)

        fragUniforms.fCameraPos = frameContext.cameraPos
        
        fragUniforms.fReceiveShadows = renderable.receiveShadows
        fragUniforms.fLightDir = scene.lighting.direction
        fragUniforms.fLightAmbientColor = scene.lighting.ambientColor
        fragUniforms.fLightDiffuseColor = scene.lighting.diffuseColor
        fragUniforms.fLightSpecularColor = scene.lighting.specularColor
        
        fragUniforms.fShadowMapSize = vector_float2(Float(scene.lighting.shadowMapSize), Float(scene.lighting.shadowMapSize))
        fragUniforms.fShadowSoftness = scene.lighting.shadowSoftness
        fragUniforms.fShadowDarkness = scene.lighting.shadowDarkness
        fragUniforms.fShadowDecayDistScaled = scene.lighting.shadowDecayDistScaled
        
        fragUniforms.fUseDepthOcclusion = false
        if occlusionPass {
            encoder.setRenderPipelineState(occlusionPipelineState)
        } else if shadowMapPass {
            vertUniforms.vViewProjMat = scene.lighting.lightSpaceMat
            encoder.setRenderPipelineState(shadowMapPipelineState)
        } else if shadowsPass {
            encoder.setRenderPipelineState(shadowsPipelineState)
        } else {
            encoder.setRenderPipelineState(defaultPipelineState)
        }

        //Logger.warn("------ VERT stride: \(MemoryLayout<StandardVertexUniforms>.stride), size \(MemoryLayout<StandardVertexUniforms>.size)")
        encoder.setVertexBytes(&vertUniforms, length: MemoryLayout<StandardVertexUniforms>.stride, index: VertexBufferIndex.VBIUniforms.rawValue)
        if numSkelJoints > 0 {
            jointTransformsBuffer.contents().copyMemory(from: Renderable_jointTransforms(renderable.coreRenderable), byteCount: MemoryLayout<simd_float4x4>.stride * numSkelJoints)
            encoder.setVertexBuffer(jointTransformsBuffer, offset: 0, index: VertexBufferIndex.VBIJointTransforms.rawValue)
        }

        encoder.setFragmentBytes(&fragUniforms, length: MemoryLayout<StandardFragmentUniforms>.stride, index: FragmentBufferIndex.FBIUniforms.rawValue)

        if baseColorMap != nil {
            encoder.setFragmentTexture(baseColorMap!.mTexture, index: FragmentTextureIndex.FTIBaseColorMap.rawValue)
        }
        if normalMap != nil {
            encoder.setFragmentTexture(normalMap!.mTexture, index: FragmentTextureIndex.FTINormalMap.rawValue)
        }
        if metalnessMap != nil {
            encoder.setFragmentTexture(metalnessMap!.mTexture, index: FragmentTextureIndex.FTIMetalnessMap.rawValue)
        }
        if roughnessMap != nil {
            encoder.setFragmentTexture(roughnessMap!.mTexture, index: FragmentTextureIndex.FTIRoughnessMap.rawValue)
        }

        //encoder.setFragmentSamplerState(texture.mSamplerState, index: FragmentTextureIndex.FTITexture.rawValue)
        if !occlusionPass && !shadowMapPass {
            encoder.setFragmentTexture(scene.shadowMap, index: FragmentTextureIndex.FTIShadowMap.rawValue)
        }

        //encoder.setFragmentTexture(depthMapTex.texture, index: FragmentTextureIndex.FTIDepthMap.rawValue)
        
        if scene.skyboxIrradianceMap != nil {
            encoder.setFragmentTexture(scene.skyboxIrradianceMap!.mTexture, index: FragmentTextureIndex.FTIIrradianceMap.rawValue)
        }

        if scene.skyboxRadianceMap != nil {
            encoder.setFragmentTexture(scene.skyboxRadianceMap!.mTexture, index: FragmentTextureIndex.FTIRadianceMap.rawValue)
        }
        fragUniforms.fRadianceMaxLod = Float(scene.skyboxRadianceMaxLod)
        
        if stencilMode != .none {
            encoder.setStencilReferenceValue(stencilRefValue)
        }
    }

    func createPipelineStateDescr(_ funcConsts: MTLFunctionConstantValues, _ mesh: Mesh) -> MTLRenderPipelineDescriptor {
        let defaultLibrary = Renderer.instance.getLibrary()
        let pipelineStateDescriptor = MTLRenderPipelineDescriptor()
        pipelineStateDescriptor.vertexFunction = try! defaultLibrary.makeFunction(name: "standard_vertex", constantValues: funcConsts)
        pipelineStateDescriptor.fragmentFunction = try! defaultLibrary.makeFunction(name: "standard_fragment", constantValues: funcConsts)
        pipelineStateDescriptor.vertexDescriptor = mesh.vertexDescriptor
        //pipelineStateDescriptor.sampleCount = Renderer.instance.view.sampleCount // deprecated in ios 16.0
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = Renderer.instance.view.colorPixelFormat
        pipelineStateDescriptor.colorAttachments[0].isBlendingEnabled = true
        pipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperation.add
        pipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperation.add
        pipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactor.sourceAlpha
        pipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactor.sourceAlpha
        pipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactor.oneMinusSourceAlpha
        pipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactor.oneMinusSourceAlpha
        pipelineStateDescriptor.depthAttachmentPixelFormat = Renderer.instance.view.depthStencilPixelFormat
        pipelineStateDescriptor.stencilAttachmentPixelFormat = Renderer.instance.view.depthStencilPixelFormat
        return pipelineStateDescriptor
    }

    func createShadowMapPipelineStateDescr(_ funcConsts: MTLFunctionConstantValues, _ mesh: Mesh) -> MTLRenderPipelineDescriptor {
        let defaultLibrary = Renderer.instance.getLibrary()
        let pipelineStateDescriptor = MTLRenderPipelineDescriptor()
        pipelineStateDescriptor.vertexFunction = try! defaultLibrary.makeFunction(name: "standard_vertex", constantValues: funcConsts)
        pipelineStateDescriptor.fragmentFunction = nil
        pipelineStateDescriptor.vertexDescriptor = mesh.vertexDescriptor
        //pipelineStateDescriptor.sampleCount = Renderer.instance.view.sampleCount // deprecated in ios 16.0
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = .invalid
        pipelineStateDescriptor.depthAttachmentPixelFormat = Renderer.instance.view.depthStencilPixelFormat
        //pipelineStateDescriptor.stencilAttachmentPixelFormat = Renderer.instance.view.depthStencilPixelFormat
        return pipelineStateDescriptor
    }
    
    func populateVertAttribsConstants(_ values: MTLFunctionConstantValues, _ mesh: Mesh) {
        // first set everyting to false
        for i in 0 ... VertexAttribType_maxIndex() {
            var tmp = false
            values.setConstantValue(&tmp, type: MTLDataType.bool, index: i)
        }
        
        // set to true only what's available
        for i in 0 ..< Mesh_numVertexAttributes(mesh.coreMesh) {
            var tmp = true
            let enumIdx = VertexAttribute_type(Mesh_vertexAttribute(mesh.coreMesh, i))
            values.setConstantValue(&tmp, type: MTLDataType.bool, index: enumIdx)
        }

        values.setConstantValue(&mesh.morphEnabled, type: MTLDataType.bool, index: FunctionConstant.FCMorphEnabled.rawValue)
        values.setConstantValue(&mesh.morphNormals, type: MTLDataType.bool, index: FunctionConstant.FCMorphNormals.rawValue)
        values.setConstantValue(&mesh.morphTangents, type: MTLDataType.bool, index: FunctionConstant.FCMorphTangents.rawValue)

        var tmp = false
        
        tmp = baseColorMap != nil
        values.setConstantValue(&tmp, type: MTLDataType.bool, index: FunctionConstant.FCBaseColorMap.rawValue)
        tmp = normalMap != nil
        values.setConstantValue(&tmp, type: MTLDataType.bool, index: FunctionConstant.FCNormalMap.rawValue)
        tmp = metalnessMap != nil
        values.setConstantValue(&tmp, type: MTLDataType.bool, index: FunctionConstant.FCMetalnessMap.rawValue)
        tmp = roughnessMap != nil
        values.setConstantValue(&tmp, type: MTLDataType.bool, index: FunctionConstant.FCRoughnessMap.rawValue)
    }
    
}
