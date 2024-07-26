import Foundation
import Metal
import DimxNative

class ImGuiPass
{
    var pipelineState2D: MTLRenderPipelineState!
    var pipelineState3D: MTLRenderPipelineState!
    
    init(_ renderer: Renderer) {
        let vertDesc = MTLVertexDescriptor()
        vertDesc.attributes[0].offset = 0
        vertDesc.attributes[0].format = MTLVertexFormat.float2
        vertDesc.attributes[0].bufferIndex = 0
        vertDesc.attributes[1].offset = 4 * 2; // two floats for pos
        vertDesc.attributes[1].format = MTLVertexFormat.float2
        vertDesc.attributes[1].bufferIndex = 0
        vertDesc.attributes[2].offset = 4 * 4; // two floats for pos and two for uv
        vertDesc.attributes[2].format = MTLVertexFormat.uchar4
        vertDesc.attributes[2].bufferIndex = 0
        vertDesc.layouts[0].stepRate = 1
        vertDesc.layouts[0].stepFunction = MTLVertexStepFunction.perVertex
        vertDesc.layouts[0].stride = 2 * 4 + 2 * 4 + 4 // pos + uv + color
        //if vertDesc.layouts[0].stride != ImGui_vertexSize() {
        //    fatalError("Invalid vertex layout!")
        //}
        
        let defaultLibrary = renderer.getLibrary()
        let pipelineStateDescriptor = MTLRenderPipelineDescriptor()
        pipelineStateDescriptor.vertexFunction = defaultLibrary.makeFunction(name: "imgui_vertex")
        pipelineStateDescriptor.fragmentFunction = defaultLibrary.makeFunction(name: "imgui_fragment")
        pipelineStateDescriptor.vertexDescriptor = vertDesc
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
        
        pipelineState2D = try! renderer.device.makeRenderPipelineState(descriptor: pipelineStateDescriptor)

        // for 3D UI
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = .rgba8Unorm
        pipelineStateDescriptor.depthAttachmentPixelFormat = .invalid
        pipelineStateDescriptor.stencilAttachmentPixelFormat = .invalid
        pipelineState3D = try! renderer.device.makeRenderPipelineState(descriptor: pipelineStateDescriptor)
    }
    
    func renderFrame(_ commandBuffer: MTLCommandBuffer?, _ defaultEncoder: MTLRenderCommandEncoder?, _ frameContext: FrameContext, _ renderer: Renderer) {
        for i in 0 ..< ImGui_numContexts() {
            let context: UnsafeRawPointer = ImGui_context(i)!
            if ImGuiContext_frameCounter(context) != frameContext.frameCounter {
                continue
            }

            var encoder: MTLRenderCommandEncoder! = nil
            let targetTexturePtr = ImGuiContext_renderTarget(context)
            if defaultEncoder != nil {
                if targetTexturePtr != nil {
                    continue
                }
                encoder = defaultEncoder!
                encoder.setRenderPipelineState(pipelineState2D)
            } else {
                if targetTexturePtr == nil {
                    continue
                }
                let targetTexture = Renderer.instance.textures[Texture_nativeId(targetTexturePtr!)]
                encoder = commandBuffer!.makeRenderCommandEncoder(descriptor: targetTexture!.mRenderPassDescriptor)
                encoder.setRenderPipelineState(pipelineState3D)
            }

            let mesh = renderer.meshes[Mesh_nativeId(ImGuiContext_mesh(context))]!
            let vertexSize = ImGuiContext_vertexSize(context)
            let indexSize = ImGuiContext_indexSize(context)
            
            encoder.setCullMode(.none)
            encoder.setVertexBuffer(mesh.vertexBuffer, offset: 0, index: 0)

            encoder.setVertexBytes(ImGuiContext_mvpMat(context), length: 4 * 16, index: 1)
            
            var totalNumVerts = 0
            var totalNumInds = 0
            
            var callVertOffset = 0
            var callIndOffset = 0
            
            var currentBufferId = -1
            for j in 0 ..< ImGuiContext_numDrawCalls(context) {
                let call: UnsafePointer<ImGuiDrawCall> = ImGuiContext_drawCall(context, j)
                
                if call.pointee.bufferId != currentBufferId {
                    let vertBufPtr = mesh.vertexBuffer.contents() + totalNumVerts * vertexSize
                    let indBufPtr = mesh.indexBuffer!.contents() + totalNumInds * indexSize
                    
                    vertBufPtr.copyMemory(from: call.pointee.vertexData,
                                          byteCount: call.pointee.numVertsInBuffer * vertexSize)
                    indBufPtr.copyMemory(from: call.pointee.indexData,
                                         byteCount: call.pointee.numIndsInBuffer * indexSize)
                    
                    callVertOffset = totalNumVerts
                    callIndOffset = totalNumInds
                    
                    totalNumVerts += call.pointee.numVertsInBuffer
                    totalNumInds += call.pointee.numIndsInBuffer
                    
                    currentBufferId = call.pointee.bufferId
                }
                
                let texId = Texture_nativeId(call.pointee.texId)
                let texture = renderer.textures[texId]!.mTexture

                // Font textures use only red channel
                var redChannel: Float = texture?.pixelFormat == MTLPixelFormat.r8Unorm ? 1.0 : 0.0
                encoder.setFragmentBytes(&redChannel, length: 4, index: 1)
                encoder.setFragmentTexture(texture, index: 0)
                encoder.drawIndexedPrimitives(type: mesh.primitiveType,
                                              indexCount: call.pointee.numIndsInCall,
                                              indexType: mesh.indexType,
                                              indexBuffer: mesh.indexBuffer!,
                                              indexBufferOffset: (callIndOffset + call.pointee.indOffsetInCall) * indexSize,
                                              instanceCount: 1,
                                              baseVertex: callVertOffset,
                                              baseInstance: 0)
            }
            if defaultEncoder == nil {
                encoder.endEncoding()
            }
        }
    }
}
