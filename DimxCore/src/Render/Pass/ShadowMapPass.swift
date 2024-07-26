import Foundation
import Metal
import DimxNative

class ShadowMapPass
{
    var queue: [[Renderable]] = [[Renderable]]()
    var renderPassDescriptor: MTLRenderPassDescriptor!
    var shadowMapTexture: MTLTexture!
    var depthStencilState: MTLDepthStencilState!
    
    init(_ renderer: Renderer) {
        let shadowMapTextureDescriptor = MTLTextureDescriptor.texture2DDescriptor(pixelFormat: .depth32Float,
                                                                               width: 2048, height: 2048,
                                                                               mipmapped: false)
        shadowMapTextureDescriptor.usage = [.shaderRead, .renderTarget]
        shadowMapTexture = renderer.device.makeTexture(descriptor: shadowMapTextureDescriptor)
        shadowMapTexture.label = "shadow map"
        
        renderPassDescriptor = MTLRenderPassDescriptor()
        let shadowAttachment = renderPassDescriptor.depthAttachment!
        shadowAttachment.texture = shadowMapTexture
        shadowAttachment.storeAction = .store

        let depthStencilDescriptor = MTLDepthStencilDescriptor()
        depthStencilDescriptor.depthCompareFunction = .less
        depthStencilDescriptor.isDepthWriteEnabled = true
        depthStencilState = renderer.device.makeDepthStencilState(descriptor: depthStencilDescriptor)
    }
    
    func enqueue(_ renderable: Renderable) {
        if renderable.castShadow {
            queue[renderable.getScene().id].append(renderable)
        }
    }

    func renderFrame(_ commandBuffer: MTLCommandBuffer, _ frameContext: FrameContext, _ renderer: Renderer) {
        for i in 0 ..< queue.count {
            let scene = renderer.scenes[i]
            if scene?.shadowMap == nil {
                continue
            }

            renderPassDescriptor.depthAttachment!.texture = scene!.shadowMap
            
            let encoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)!
            encoder.pushDebugGroup("shadow pass " + String(i))
            encoder.label = "shadow map " + String(i)
            encoder.setCullMode(.back)
            encoder.setFrontFacing(.counterClockwise)
            encoder.setDepthStencilState(depthStencilState)
            
            for rend in queue[i] {
                for mesh in rend.meshes {
                    mesh.material.setupRender(renderer, encoder, mesh, frameContext, occlusionPass: false, shadowMapPass: true, shadowsPass: false)
                    mesh.mesh.draw(encoder)
                }
            }
            queue[i].removeAll()
            
            encoder.popDebugGroup()
            encoder.endEncoding()
        }
    }
    
    func resizeScenesQueue(_ count: Int) {
        while count > queue.count {
            queue.append([])
        }
    }
}
