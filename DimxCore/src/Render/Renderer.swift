import UIKit
import Metal
import MetalKit
import DimxNative

func validateVertexAttributeEnum()
{
    /*
     See ShaderCommon.h
     VertexAttribute and FunctionConstant must match core VertexAttribType!
     This is required to match vertex attributes to shader inputs.
     */
    if (VertexAttribute.vPosition.rawValue      != VertexAttribType_index("vPosition"))            { fatalError("VertexAttribute mismatch") }
    if (VertexAttribute.vPosition.rawValue      != FunctionConstant.FCPositionAttr.rawValue)       { fatalError("VertexAttribute mismatch") }
    
    if (VertexAttribute.vPosition2.rawValue     != VertexAttribType_index("vPosition2"))           { fatalError("VertexAttribute mismatch") }
    if (VertexAttribute.vPosition2.rawValue     != FunctionConstant.FCPosition2Attr.rawValue)      { fatalError("VertexAttribute mismatch") }
    
    if (VertexAttribute.vNormal.rawValue        != VertexAttribType_index("vNormal"))              { fatalError("VertexAttribute mismatch") }
    if (VertexAttribute.vNormal.rawValue        != FunctionConstant.FCNormalAttr.rawValue)         { fatalError("VertexAttribute mismatch") }
    
    if (VertexAttribute.vTangent.rawValue       != VertexAttribType_index("vTangent"))             { fatalError("VertexAttribute mismatch") }
    if (VertexAttribute.vTangent.rawValue       != FunctionConstant.FCTangentAttr.rawValue)        { fatalError("VertexAttribute mismatch") }
    
    if (VertexAttribute.vBitangent.rawValue     != VertexAttribType_index("vBitangent"))           { fatalError("VertexAttribute mismatch") }
    if (VertexAttribute.vBitangent.rawValue     != FunctionConstant.FCBitangentAttr.rawValue)      { fatalError("VertexAttribute mismatch") }
    
    if (VertexAttribute.vTexCoord.rawValue      != VertexAttribType_index("vTexCoord"))            { fatalError("VertexAttribute mismatch") }
    if (VertexAttribute.vTexCoord.rawValue      != FunctionConstant.FCTexCoordAttr.rawValue)       { fatalError("VertexAttribute mismatch") }
    
    if (VertexAttribute.vColor.rawValue         != VertexAttribType_index("vColor"))               { fatalError("VertexAttribute mismatch") }
    if (VertexAttribute.vColor.rawValue         != FunctionConstant.FCColorAttr.rawValue)          { fatalError("VertexAttribute mismatch") }
    
    if (VertexAttribute.vColorUB.rawValue       != VertexAttribType_index("vColorUB"))             { fatalError("VertexAttribute mismatch") }
    if (VertexAttribute.vColorUB.rawValue       != FunctionConstant.FCColorUBAttr.rawValue)        { fatalError("VertexAttribute mismatch") }
    
    if (VertexAttribute.vJointIndex.rawValue    != VertexAttribType_index("vJointIndex"))          { fatalError("VertexAttribute mismatch") }
    if (VertexAttribute.vJointIndex.rawValue    != FunctionConstant.FCJointIndexAttr.rawValue)     { fatalError("VertexAttribute mismatch") }
    
    if (VertexAttribute.vJointIndices4.rawValue != VertexAttribType_index("vJointIndices4"))       { fatalError("VertexAttribute mismatch") }
    if (VertexAttribute.vJointIndices4.rawValue != FunctionConstant.FCJointIndices4Attr.rawValue)  { fatalError("VertexAttribute mismatch") }
    
    if (VertexAttribute.vJointWeights4.rawValue != VertexAttribType_index("vJointWeights4"))       { fatalError("VertexAttribute mismatch") }
    if (VertexAttribute.vJointWeights4.rawValue != FunctionConstant.FCJointWeights4Attr.rawValue)  { fatalError("VertexAttribute mismatch") }
    
    if (VertexAttribute.vNone.rawValue          != VertexAttribType_index("None"))                 { fatalError("VertexAttribute mismatch") }
    if (VertexAttribute.vNone.rawValue          != FunctionConstant.FCNoneAttr.rawValue)           { fatalError("VertexAttribute mismatch") }
}

class Renderer
{
    static /*private*/ let instance = Renderer()
    private init() {}

    var view: MTKView!

    var device: MTLDevice!
    var commandQueue: MTLCommandQueue!
    var depthStencilState: MTLDepthStencilState!
    
    var depthStencilTexture: MTLTexture?
    
    var textures: [Texture?] = []
    var materials: [Material?] = []
    var meshes: [Mesh?] = []
    var renderables: [Renderable?] = []
    var scenes: [Scene?] = []

    var backgroundPass: BackgroundPass!
    var occlusionPass: OcclusionPass!
    var defaultPass: DefaultPass!
    var imguiPass: ImGuiPass!
    var debugPass: DebugPass!
    var shadowMapPass: ShadowMapPass!
    var shadowsPass: ShadowsPass!
    
    var frameContext = FrameContext()
    
    var viewportSize: CGSize { get {view.currentDrawable!.layer.drawableSize} }

    static func initCallbacks() {
        g_swiftRenderer().pointee.initialize = { (configPtr: Optional<UnsafeRawPointer>) -> () in Renderer.instance.initialize(configPtr!) }
        g_swiftRenderer().pointee.postInit = { (configPtr: Optional<UnsafeRawPointer>) -> () in Renderer.instance.postInit(configPtr!) }
        g_swiftRenderer().pointee.beginFrame = { (contextPtr: Optional<UnsafeRawPointer>) -> () in Renderer.instance.beginFrame(contextPtr!) }
        g_swiftRenderer().pointee.endFrame = { Renderer.instance.endFrame() }
        g_swiftRenderer().pointee.getFrameImageData = { (width: Int, height: Int, outPtr: Optional<UnsafeMutableRawPointer>) -> () in Renderer.instance.getFrameImageData(width, height, outPtr!) }
        
        g_swiftRenderer().pointee.createScene = { (ptr: Optional<UnsafeRawPointer>) -> (Int) in return Renderer.instance.createScene(ptr!) }
        g_swiftRenderer().pointee.deleteScene = { (id: Int) -> () in return Renderer.instance.deleteScene(id) }
/*
        testCallback({
            (meshPtr: Optional<UnsafeRawPointer>) -> (Int) in
            return Renderer.instance.createMesh(meshPtr: meshPtr!)
        })
*/
    }
    
    static func initSingleton(_ device: MTLDevice, _ view: MTKView) {
        if instance.device != nil {
            fatalError("View already assigned")
        }
        instance.device = device
        instance.view = view

        validateVertexAttributeEnum()
    }

    func initialize(_ configPtr: UnsafeRawPointer) {
        Logger.info("renderer initalize")
        commandQueue = device.makeCommandQueue()

        let depthStencilDescriptor = MTLDepthStencilDescriptor()
        depthStencilDescriptor.depthCompareFunction = .lessEqual
        depthStencilDescriptor.isDepthWriteEnabled = true
        depthStencilState = device.makeDepthStencilState(descriptor: depthStencilDescriptor)

        let stencilTexDescr = MTLTextureDescriptor.texture2DDescriptor(
            pixelFormat: .depth32Float_stencil8, width: Int(viewportSize.width), height: Int(viewportSize.height), mipmapped: false)
        stencilTexDescr.usage = [.renderTarget]
        stencilTexDescr.storageMode = .private
        depthStencilTexture = device.makeTexture(descriptor: stencilTexDescr)!
        
        backgroundPass = BackgroundPass(self)
        occlusionPass = OcclusionPass(self)
        defaultPass = DefaultPass(self)
        debugPass = DebugPass(self)
        shadowMapPass = ShadowMapPass(self)
        shadowsPass = ShadowsPass(self)
        
    }
    
    func postInit(_ configPtr: UnsafeRawPointer) {
        /*
        backgroundPass = BackgroundPass(renderer: self)
        occlusionPass = OcclusionPass(renderer: self)
        defaultPass = DefaultPass(renderer: self)
        debugPass = DebugPass(renderer: self)
        shadowMapPass = ShadowMapPass(renderer: self)
        shadowsPass = ShadowsPass(renderer: self)
        */
    }

    func beginFrame(_ frameContextPtr: UnsafeRawPointer) {
        if imguiPass == nil {
            imguiPass = ImGuiPass(self)
        }

        frameContext.populateFromCore(ptr: frameContextPtr)
    }
    
    func endFrame() {
        for scene in scenes {
            if (scene != nil) {
                scene!.onFrameUpdate()
            }
        }

        let commandBuffer = commandQueue.makeCommandBuffer()!

        imguiPass.renderFrame(commandBuffer, nil, frameContext, self)
        
        shadowMapPass.renderFrame(commandBuffer, frameContext, self)

        guard let passDescriptor = view.currentRenderPassDescriptor else { fatalError("Failed to get currentRenderPassDescriptor") }
        passDescriptor.colorAttachments[0].loadAction = .clear
        
        passDescriptor.depthAttachment.texture = depthStencilTexture
        passDescriptor.depthAttachment.loadAction = .clear
        passDescriptor.depthAttachment.storeAction = .dontCare
        passDescriptor.depthAttachment.clearDepth = 1.0
        
        passDescriptor.stencilAttachment.texture = depthStencilTexture
        passDescriptor.stencilAttachment.loadAction = .clear
        passDescriptor.stencilAttachment.storeAction = .dontCare
        passDescriptor.stencilAttachment.clearStencil = 0
        
        let commandEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: passDescriptor)!
        commandEncoder.setFrontFacing(.counterClockwise)

        backgroundPass.renderFrame(commandEncoder, frameContext, self)

        commandEncoder.setCullMode(.back)
        commandEncoder.setDepthStencilState(depthStencilState)
        
        occlusionPass.renderFrame(commandEncoder, frameContext, self)
        defaultPass.renderFrame(commandEncoder, frameContext, self)
        shadowsPass.renderFrame(commandEncoder, frameContext, self)
        debugPass.renderFrame(commandEncoder, frameContext, self)
        imguiPass.renderFrame(nil, commandEncoder, frameContext, self)

        commandEncoder.endEncoding()

        commandBuffer.present(view.currentDrawable!)
        commandBuffer.commit()
    }

    func getFrameImageData(_ width: Int, _ height: Int, _ outPtr: UnsafeMutableRawPointer) {
        guard let drawable = view.currentDrawable else {
            return
        }
        
        let texture = drawable.texture
        if texture.width != width || texture.height != height {
            Logger.error("getFrameImageData: invalid texture size \(texture.width)x\(texture.height), expected \(width)x\(height)")
        }
        
        let width = texture.width
        let height = texture.height
        
        let bytesPerPixel = 4 // RGBA
        let bytesPerRow = width * bytesPerPixel
        
        let region = MTLRegionMake2D(0, 0, width, height)
        texture.getBytes(outPtr, bytesPerRow: bytesPerRow, from: region, mipmapLevel: 0)
    }
    
    func render(_ renderable: Renderable) {
        occlusionPass.enqueue(renderable)
        defaultPass.enqueue(renderable)
        if Settings_displayShadows() {
            shadowMapPass.enqueue(renderable)
            shadowsPass.enqueue(renderable)
        }
    }
    
    func createScene(_ ptr: UnsafeRawPointer) -> Int {
        // ShadowMapPass iterates through all scenes.
        // So we reuse free slots of deleted scenes.
        
        var availableIdx = -1
        for i in 0 ..< scenes.count {
            if (scenes[i] == nil && availableIdx == -1) {
                availableIdx = i
            }
            if(scenes[i] != nil && scenes[i]!.coreId() == Scene_id(ptr)) {
                return i
            }
        }

        if (availableIdx == -1) {
            availableIdx = scenes.count
        }
        
        let scene = Scene(availableIdx, self, ptr)
        
        if (availableIdx < scenes.count) {
            scenes[availableIdx] = scene
        } else {
            scenes.append(scene)
            if (availableIdx != scenes.count - 1) {
                fatalError("Bad available scene index")
            }
        }
        
        shadowMapPass.resizeScenesQueue(scenes.count)
        
        return availableIdx
    }
    
    func deleteScene(_ id: Int) {
        scenes[Int(id)] = nil
    }

    func shadowMapTexture() -> MTLTexture {
        return shadowMapPass.shadowMapTexture
    }

    func getLibrary() -> MTLLibrary {
        do {
            return try device.makeDefaultLibrary(bundle: Bundle.module)
        } catch {
            fatalError("Failed to create default MTLLibrary: \(error)")
        }
    }
}
