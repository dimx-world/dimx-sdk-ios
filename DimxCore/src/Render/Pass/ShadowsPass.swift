import Foundation
import Metal
import DimxNative

class ShadowsPass
{
    var queue: [Renderable] = []
    
    init(_ renderer: Renderer) {
    }
    
    func enqueue(_ renderable: Renderable) {
        if renderable.shadowPass && renderable.receiveShadows {
            queue.append(renderable)
        }
    }

    func renderFrame(_ encoder: MTLRenderCommandEncoder, _ frameContext: FrameContext, _ renderer: Renderer) {
        for rend in queue {
            for mesh in rend.meshes {
                mesh.material.setupRender(renderer, encoder, mesh, frameContext, occlusionPass: false, shadowMapPass: false, shadowsPass: true)
                mesh.mesh.draw(encoder)
            }
        }
        queue.removeAll()
    }
}
