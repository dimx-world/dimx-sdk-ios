import Foundation
import Metal
import ARKit
import DimxNative

class DebugPass
{
    static let componentsPerVertex = 7 // xyz + rgba
    static let vertexSize = MemoryLayout<Float>.stride * componentsPerVertex
    static let matrixSize = MemoryLayout<Float>.stride * 16

    static let maxLinesVerts = 1000
    static let maxLinesDataSize = maxLinesVerts * vertexSize
    
    static let maxPolysVerts = 6000
    static let maxPolysDataSize = maxPolysVerts * vertexSize
    
    var linesVertexBuffer: MTLBuffer!
    var polysVertexBuffer: MTLBuffer!

    var debugLinesVertexBuffer: MTLBuffer!
    var debugPolysVertexBuffer: MTLBuffer!
    
    var pipelineState: MTLRenderPipelineState!

    init(_ renderer: Renderer) {
        linesVertexBuffer = renderer.device.makeBuffer(length: DebugPass.maxLinesDataSize, options: [])
        polysVertexBuffer = renderer.device.makeBuffer(length: DebugPass.maxPolysDataSize, options: [])
        debugLinesVertexBuffer = renderer.device.makeBuffer(length: DebugPass.maxLinesDataSize, options: [])
        debugPolysVertexBuffer = renderer.device.makeBuffer(length: DebugPass.maxPolysDataSize, options: [])
        
        let vertexDescriptor = MTLVertexDescriptor()
        vertexDescriptor.attributes[0].format = .float3
        vertexDescriptor.attributes[0].offset = 0
        vertexDescriptor.attributes[0].bufferIndex = 0
        vertexDescriptor.attributes[1].format = .float4
        vertexDescriptor.attributes[1].offset = 12 // offset after xyz
        vertexDescriptor.attributes[1].bufferIndex = 0
        vertexDescriptor.layouts[0].stride = DebugPass.vertexSize
        vertexDescriptor.layouts[0].stepRate = 1
        vertexDescriptor.layouts[0].stepFunction = .perVertex

        let defaultLibrary = renderer.getLibrary()
        let pipelineStateDescriptor = MTLRenderPipelineDescriptor()
        pipelineStateDescriptor.label = "DebugPipeline"
        //pipelineStateDescriptor.sampleCount = renderer.view.sampleCount
        pipelineStateDescriptor.vertexFunction = defaultLibrary.makeFunction(name: "debug_vertex")!
        pipelineStateDescriptor.fragmentFunction = defaultLibrary.makeFunction(name: "debug_fragment")!
        pipelineStateDescriptor.vertexDescriptor = vertexDescriptor
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = renderer.view.colorPixelFormat
        pipelineStateDescriptor.colorAttachments[0].isBlendingEnabled = true
        pipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperation.add
        pipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperation.add
        pipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactor.sourceAlpha
        pipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactor.sourceAlpha
        pipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactor.oneMinusSourceAlpha
        pipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactor.oneMinusSourceAlpha
        pipelineStateDescriptor.depthAttachmentPixelFormat = renderer.view.depthStencilPixelFormat
        pipelineStateDescriptor.stencilAttachmentPixelFormat = renderer.view.depthStencilPixelFormat
        do { try pipelineState = renderer.device.makeRenderPipelineState(descriptor: pipelineStateDescriptor) }
        catch let error { Logger.error("Failed to create debug lines pipeline state, error \(error)") }
    }
    
    func renderFrame(_ encoder: MTLRenderCommandEncoder, _ frameContext: FrameContext, _ renderer: Renderer) {
        let linesVertexCount = min(Renderer_linesVertsCount(false), DebugPass.maxLinesVerts)
        let polysVertexCount = min(Renderer_linesPolyVertsCount(false), DebugPass.maxPolysVerts)
        let debugLinesVertexCount = min(Renderer_linesVertsCount(true), DebugPass.maxLinesVerts)
        let debugPolysVertexCount = min(Renderer_linesPolyVertsCount(true), DebugPass.maxPolysVerts)
        
        if linesVertexCount == 0 && polysVertexCount == 0 && debugLinesVertexCount == 0 && debugPolysVertexCount == 0 {
            return
        }
        
        encoder.setCullMode(.none)
        encoder.setRenderPipelineState(pipelineState)
        encoder.setVertexBytes(&frameContext.viewProjectionMat, length: DebugPass.matrixSize, index: 1)

        if linesVertexCount > 0 {
            linesVertexBuffer.contents().copyMemory(from: Renderer_linesVertsData(false), byteCount: DebugPass.vertexSize * linesVertexCount)
            encoder.setVertexBuffer(linesVertexBuffer, offset: 0, index: 0)
            encoder.drawPrimitives(type: .line, vertexStart: 0, vertexCount: linesVertexCount)
        }
        if polysVertexCount > 0 {
            polysVertexBuffer.contents().copyMemory(from: Renderer_linesPolyVertsData(false), byteCount: DebugPass.vertexSize * polysVertexCount)
            encoder.setVertexBuffer(polysVertexBuffer, offset: 0, index: 0)
            encoder.drawPrimitives(type: .triangle , vertexStart: 0, vertexCount: polysVertexCount)
        }
        
        if debugLinesVertexCount > 0 {
            debugLinesVertexBuffer.contents().copyMemory(from: Renderer_linesVertsData(true), byteCount: DebugPass.vertexSize * debugLinesVertexCount)
            encoder.setVertexBuffer(debugLinesVertexBuffer, offset: 0, index: 0)
            encoder.drawPrimitives(type: .line, vertexStart: 0, vertexCount: debugLinesVertexCount)
        }
        if debugPolysVertexCount > 0 {
            debugPolysVertexBuffer.contents().copyMemory(from: Renderer_linesPolyVertsData(true), byteCount: DebugPass.vertexSize * debugPolysVertexCount)
            encoder.setVertexBuffer(debugPolysVertexBuffer, offset: 0, index: 0)
            encoder.drawPrimitives(type: .triangle , vertexStart: 0, vertexCount: debugPolysVertexCount)
        }
    }
    
    func drawLine(_ p1: simd_float3, _ p2: simd_float3, _ color: simd_float4)
    {
        var tmpP1 = p1;
        var tmpP2 = p2;
        var tmpCol = color;
        Renderer_drawLine(true, &tmpP1, &tmpCol, &tmpP2, &tmpCol)
    }

    func drawTriangle(_ v1: simd_float3, _ v2: simd_float3, _ v3: simd_float3, _ color: simd_float4) {
        var tmpV1 = v1;
        var tmpV2 = v2;
        var tmpV3 = v3;
        var tmpCol = color;
        Renderer_drawTriangle(true, &tmpV1, &tmpV2, &tmpV3, &tmpCol)
    }

    func drawRect(_ width: Float, _ height: Float, _ transform: simd_float4x4, _ color: simd_float4) {
        var tmpTrans = transform;
        var tmpCol = color;
        Renderer_drawLineRect(true, width, height, &tmpTrans, &tmpCol)
    }
   
    func drawPlane(_ plane: ARPlaneGeometry, _ transform: simd_float4x4, _ color: simd_float4) {
        var verts = plane.vertices
        for i in 0..<verts.count {
            let v = transform * simd_float4(verts[i].x, verts[i].y, verts[i].z, 1)
            verts[i] = simd_float3(v.x, v.y, v.z)
        }
        let inds = plane.triangleIndices
        for i in 0 ..< plane.triangleCount {
            drawTriangle(verts[Int(inds[i * 3])], verts[Int(inds[i * 3 + 1])], verts[Int(inds[i * 3 + 2])], color)
        }
    }
    
    func drawPoint(_ pos: simd_float3, _ size: Float,_ color: simd_float4) {
        if DeviceAR.instance.currentFrame() == nil {
            return
        }
        let camMat = DeviceAR.instance.currentFrame()!.camera.transform
        let s2 = size * 0.5
        
        let vert = {(x: Float, y: Float) -> simd_float3 in
            let t = camMat * simd_float4(x, y, 0, 0)
            return simd_float3(t.x, t.y, t.z) + pos
        }

        drawTriangle(vert(-s2,  s2), vert(-s2, -s2), vert(s2, s2), color)
        drawTriangle(vert(-s2, -s2), vert( s2, -s2), vert(s2, s2), color)
    }
}
