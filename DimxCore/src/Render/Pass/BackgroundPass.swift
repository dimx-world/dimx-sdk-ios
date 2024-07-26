import Foundation
import Metal
import ARKit
import DimxNative

class BackgroundPass
{
    // Reference: https://github.com/MetalKit/metal/blob/master/ARKit/arkitdemo/Renderer.swift
    let planeVertexData: [Float] = [-1, -1,  0,  1,
                                     1, -1,  1,  1,
                                    -1,  1,  0,  0,
                                     1,  1,  1,  0]

    var vertexBuffer: MTLBuffer!
    var pipelineState: MTLRenderPipelineState!
    var depthState: MTLDepthStencilState!
    var textureY: MTLTexture!
    var textureCbCr: MTLTexture!
    var textureCache: CVMetalTextureCache!
    
    var viewportSizeDidChange = true;

    init(_ renderer: Renderer) {
        vertexBuffer = renderer.device.makeBuffer(bytes: planeVertexData, length: planeVertexData.count * MemoryLayout<Float>.size, options: [])
        
        let vertexDescriptor = MTLVertexDescriptor()
        vertexDescriptor.attributes[0].format = .float2
        vertexDescriptor.attributes[0].offset = 0
        vertexDescriptor.attributes[0].bufferIndex = 0
        vertexDescriptor.attributes[1].format = .float2
        vertexDescriptor.attributes[1].offset = 8
        vertexDescriptor.attributes[1].bufferIndex = 0
        vertexDescriptor.layouts[0].stride = 16
        vertexDescriptor.layouts[0].stepRate = 1
        vertexDescriptor.layouts[0].stepFunction = .perVertex

        let defaultLibrary = renderer.getLibrary()
        let pipelineStateDescriptor = MTLRenderPipelineDescriptor()
        pipelineStateDescriptor.label = "VideoBackgroundPipeline"
        //pipelineStateDescriptor.sampleCount = renderer.view.sampleCount
        pipelineStateDescriptor.vertexFunction = defaultLibrary.makeFunction(name: "background_vertex")!
        pipelineStateDescriptor.fragmentFunction = defaultLibrary.makeFunction(name: "background_fragment")!
        pipelineStateDescriptor.vertexDescriptor = vertexDescriptor
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = renderer.view.colorPixelFormat
        pipelineStateDescriptor.depthAttachmentPixelFormat = renderer.view.depthStencilPixelFormat
        pipelineStateDescriptor.stencilAttachmentPixelFormat = renderer.view.depthStencilPixelFormat
        do { try pipelineState = renderer.device.makeRenderPipelineState(descriptor: pipelineStateDescriptor) }
        catch let error { Logger.error("Failed to created captured image pipeline state, error \(error)") }

        let depthStateDescriptor = MTLDepthStencilDescriptor()
        depthStateDescriptor.depthCompareFunction = .less//.always
        depthStateDescriptor.isDepthWriteEnabled = false
        depthState = renderer.device.makeDepthStencilState(descriptor: depthStateDescriptor)

        var tmpCache: CVMetalTextureCache?
        CVMetalTextureCacheCreate(nil, nil, renderer.device, nil, &tmpCache)
        textureCache = tmpCache
    }

    func createTexture(_ pixelBuffer: CVPixelBuffer, _ pixelFormat: MTLPixelFormat, _ planeIndex: Int) -> MTLTexture? {
        var mtlTexture: MTLTexture? = nil
        let width = CVPixelBufferGetWidthOfPlane(pixelBuffer, planeIndex)
        let height = CVPixelBufferGetHeightOfPlane(pixelBuffer, planeIndex)
        var texture: CVMetalTexture? = nil
        let status = CVMetalTextureCacheCreateTextureFromImage(nil, textureCache, pixelBuffer, nil, pixelFormat, width, height, planeIndex, &texture)
        if status == kCVReturnSuccess { mtlTexture = CVMetalTextureGetTexture(texture!) }
        return mtlTexture
    }
    
    func renderFrame(_ encoder: MTLRenderCommandEncoder, _ frameContext: FrameContext, _ renderer: Renderer) {
        let currentFrame = DeviceAR.instance.currentFrame()
        
        if currentFrame != nil {
            let pixelBuffer = currentFrame!.capturedImage
            if (CVPixelBufferGetPlaneCount(pixelBuffer) < 2) { fatalError("Invalid pixel buffer plane count") }
            textureY = createTexture(pixelBuffer, .r8Unorm, 0)!
            textureCbCr = createTexture(pixelBuffer, .rg8Unorm, 1)!
            
            if viewportSizeDidChange {
                // do once
                viewportSizeDidChange = false
                updateImagePlane(currentFrame!, renderer)
            }
        }
        
        guard textureY != nil && textureCbCr != nil else { return }
        encoder.pushDebugGroup("DrawCapturedImage")
        encoder.setCullMode(.none)
        encoder.setDepthStencilState(depthState)
        encoder.setRenderPipelineState(pipelineState)
        encoder.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        encoder.setFragmentTexture(textureY, index: 1)
        encoder.setFragmentTexture(textureCbCr, index: 2)
        encoder.drawPrimitives(type: .triangleStrip, vertexStart: 0, vertexCount: 4)
        encoder.popDebugGroup()
    }
    
    func updateImagePlane(_ frame: ARFrame, _ renderer: Renderer) {
        let displayToCameraTransform = frame.displayTransform(for: Context.inst().getInterfaceOrientation(), viewportSize: renderer.viewportSize).inverted()
        
        Logger.info("BackgroundPass::updateImagePlane \(Context.inst().getInterfaceOrientation()), \(renderer.viewportSize)")
        
        let vertexData = vertexBuffer.contents().assumingMemoryBound(to: Float.self)
        for index in 0...3 {
            let textureCoordIndex = 4 * index + 2
            let textureCoord = CGPoint(x: CGFloat(planeVertexData[textureCoordIndex]), y: CGFloat(planeVertexData[textureCoordIndex + 1]))
            let transformedCoord = textureCoord.applying(displayToCameraTransform)
            vertexData[textureCoordIndex] = Float(transformedCoord.x)
            vertexData[textureCoordIndex + 1] = Float(transformedCoord.y)
        }
    }
    
    func resetTexture() {
        textureY = nil
        textureCbCr = nil
    }
    
    func setViewportChanged() {
        viewportSizeDidChange = true
    }
}
