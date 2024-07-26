import Foundation
import Metal
import DimxNative

class OcclusionPass
{
    var queue: [Renderable] = []
    
    init(_ renderer: Renderer) {
    }
    
    func enqueue(_ renderable: Renderable) {
        if renderable.occlusion {
            queue.append(renderable)
        }
    }

    func renderFrame(_ encoder: MTLRenderCommandEncoder, _ frameContext: FrameContext, _ renderer: Renderer) {
        for rend in queue {
            for mesh in rend.meshes {
                mesh.material.setupRender(renderer, encoder, mesh, frameContext, occlusionPass: true, shadowMapPass: false, shadowsPass: false)
                mesh.mesh.draw(encoder)
            }
        }
        
        queue.removeAll()
    }
}
