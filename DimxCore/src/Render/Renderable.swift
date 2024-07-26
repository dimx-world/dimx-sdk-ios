import Foundation
import Metal
import simd
import DimxNative
class Renderable
{
   
    var meshes: [RenderableMesh] = []
    let coreRenderable: UnsafeRawPointer
    private var scene: Scene?
    
    var castShadow = false
    var receiveShadows = false
    var occlusion = false
    var shadowPass = false

    static func initCallbacks() {
        g_swiftRenderable().pointee.createRenderable = { (coreRend: Optional<UnsafeRawPointer>) -> (Int) in return Renderable.create(coreRend!) }
        g_swiftRenderable().pointee.deleteRenderable = { (id: Int) -> () in Renderable.delete(id) }
        g_swiftRenderable().pointee.render = { (id: Int) -> () in Renderer.instance.render(Renderer.instance.renderables[id]!) }
        g_swiftRenderable().pointee.setHighlightFactor = {
            (id: Int, factor: Float) -> () in
            Renderer.instance.renderables[id]!.setHighlightFactor(factor)
        }
        g_swiftRenderable().pointee.setCastShadow = {
            (id: Int, value: Bool) -> () in
            Renderer.instance.renderables[id]!.castShadow = value
        }
        g_swiftRenderable().pointee.setReceiveShadows = {
            (id: Int, value: Bool) -> () in
            Renderer.instance.renderables[id]!.receiveShadows = value
        }
        g_swiftRenderable().pointee.setShadowPass = {
            (id: Int, value: Bool) -> () in
            Renderer.instance.renderables[id]!.shadowPass = value
        }
        g_swiftRenderable().pointee.setOcclusion = {
            (id: Int, value: Bool) -> () in
            Renderer.instance.renderables[id]!.occlusion = value
        }
    }

    static func create(_ coreRend: UnsafeRawPointer) -> Int {
        Renderer.instance.renderables.append(Renderable(coreRend))
        return Renderer.instance.renderables.count - 1
    }
    
    static func delete(_ id: Int) {
        Renderer.instance.renderables[id] = nil
    }
    
    init(_ coreRend: UnsafeRawPointer) {
        coreRenderable = coreRend
        for i in 0 ..< Renderable_numMeshes(coreRend) {
            let meshPtr = Renderable_mesh(coreRend, i)!
            meshes.append(RenderableMesh(self, meshPtr))
        }
        
        castShadow = Renderable_castShadow(coreRend)
        receiveShadows = Renderable_receiveShadows(coreRend)
        occlusion = Renderable_occlusion(coreRend)
        shadowPass = Renderable_shadowPass(coreRend)
    }

    func nodeTransform() -> matrix_float4x4 {
        var tmp = matrix_float4x4()
        Renderable_nodeTransform(coreRenderable, &tmp)
        return tmp
    }
    
    func nodeNormalTransform() -> matrix_float3x3 {
        var tmp = matrix_float3x3()
        Renderable_nodeNormalTransform(coreRenderable, &tmp)
        return tmp
    }
    
    func numSkelJoints() -> Int {
        return Renderable_numSkeJoints(coreRenderable)
    }
    
    func morphNumTargets() -> Int32 {
        return Int32(Renderable_morphNumTargets(coreRenderable))
    }
    
    func morphTargetIds() -> UnsafeRawPointer {
        return Renderable_morphTargetIds(coreRenderable)
    }

    func morphTargetWeights() -> UnsafeRawPointer {
        return Renderable_morphTargetWeights(coreRenderable)
    }
    
    func setHighlightFactor(_ factor: Float) {
        for info in meshes {
            info.material.highlightFactor = factor
        }
    }
    
    func getScene() -> Scene {
        if scene == nil {
            scene = Renderer.instance.scenes[Int(Renderable_sceneRenderId(coreRenderable))]
            if scene == nil {
                fatalError("Nil render scene")
            }
        }
        return scene!
    }
}
