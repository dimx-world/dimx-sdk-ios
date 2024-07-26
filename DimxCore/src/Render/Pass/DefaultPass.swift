import Foundation
import Metal
import DimxNative

class DefaultPass
{
    var opaque: [RenderableMesh] = []
    var transparent: [RenderableMesh] = []
    var stencil: [RenderableMesh] = []
    
    var writeStencilState: MTLDepthStencilState?
    var readStencilState: MTLDepthStencilState?
    
    init(_ renderer: Renderer) {
        let writeStencilDescr = MTLDepthStencilDescriptor()
        writeStencilDescr.frontFaceStencil = MTLStencilDescriptor()
        writeStencilDescr.isDepthWriteEnabled = false
        writeStencilDescr.frontFaceStencil.stencilCompareFunction = .always
        writeStencilDescr.frontFaceStencil.stencilFailureOperation = .keep
        writeStencilDescr.frontFaceStencil.depthFailureOperation = .keep
        writeStencilDescr.frontFaceStencil.depthStencilPassOperation = .replace
        writeStencilDescr.frontFaceStencil.readMask = 0xFF
        writeStencilDescr.frontFaceStencil.writeMask = 0xFF
        writeStencilState = Renderer.instance.device.makeDepthStencilState(descriptor: writeStencilDescr)!
        
        let readStencilDescr = MTLDepthStencilDescriptor()
        readStencilDescr.frontFaceStencil = MTLStencilDescriptor()
        readStencilDescr.frontFaceStencil.stencilCompareFunction = .equal
        readStencilDescr.frontFaceStencil.stencilFailureOperation = .keep
        readStencilDescr.frontFaceStencil.depthFailureOperation = .keep
        readStencilDescr.frontFaceStencil.depthStencilPassOperation = .keep
        readStencilDescr.frontFaceStencil.readMask = 0xFF
        readStencilDescr.frontFaceStencil.writeMask = 0x00
        readStencilState = Renderer.instance.device.makeDepthStencilState(descriptor: readStencilDescr)!
    }
    
    func enqueue(_ renderable: Renderable) {
        if (renderable.occlusion || renderable.shadowPass) && !Settings_displayOcclusionObjects() {
            return;
        }

        for mesh in renderable.meshes {
            if mesh.material.stencilMode == .write {
                stencil.append(mesh)
            } else if mesh.material.transparent {
                // Maintain sorted order fom back to front towards the camera
                if let index = transparent.firstIndex(where: { $0.cameraDistSq() < mesh.cameraDistSq() }) {
                    transparent.insert(mesh, at: index)
                } else {
                    transparent.append(mesh)
                }
            } else {
                opaque.append(mesh)
            }
        }
    }

    func renderFrame(_ encoder: MTLRenderCommandEncoder, _ frameContext: FrameContext, _ renderer: Renderer) {
        
        if stencil.count > 0 {
            encoder.setDepthStencilState(writeStencilState)
            for mesh in stencil {
                drawRenderable(mesh, encoder, frameContext, renderer)
            }
            stencil.removeAll()
            encoder.setDepthStencilState(renderer.depthStencilState)
        }
        
        for mesh in opaque {
            drawRenderable(mesh, encoder, frameContext, renderer)
        }
        opaque.removeAll()

        for mesh in transparent {
            drawRenderable(mesh, encoder, frameContext, renderer)
        }
        transparent.removeAll()
    }
    
    func drawRenderable(_ mesh: RenderableMesh, _ encoder: MTLRenderCommandEncoder, _ frameContext: FrameContext, _ renderer: Renderer) {
        if mesh.material.stencilMode == .read {
            encoder.setDepthStencilState(readStencilState)
        }

        mesh.material.setupRender(renderer, encoder, mesh, frameContext, occlusionPass: false, shadowMapPass: false, shadowsPass: false)
        mesh.mesh.draw(encoder)
        
        if mesh.material.stencilMode == .read {
            encoder.setDepthStencilState(renderer.depthStencilState)
        }
    }
    
    func createStencilStateDescriptor(_ mode: StencilMode, _ cmpFunc: MTLCompareFunction) -> MTLDepthStencilDescriptor
    {
        let descriptor = MTLDepthStencilDescriptor()
        descriptor.frontFaceStencil = MTLStencilDescriptor()
        if mode == .write {
            descriptor.isDepthWriteEnabled = false
            descriptor.frontFaceStencil.stencilCompareFunction = .always
            descriptor.frontFaceStencil.stencilFailureOperation = .keep
            descriptor.frontFaceStencil.depthFailureOperation = .keep
            descriptor.frontFaceStencil.depthStencilPassOperation = .replace
            descriptor.frontFaceStencil.readMask = 0xFF
            descriptor.frontFaceStencil.writeMask = 0xFF
        } else if mode == .read {
            descriptor.frontFaceStencil.stencilCompareFunction = cmpFunc
            descriptor.frontFaceStencil.stencilFailureOperation = .keep
            descriptor.frontFaceStencil.depthFailureOperation = .keep
            descriptor.frontFaceStencil.depthStencilPassOperation = .keep
            descriptor.frontFaceStencil.readMask = 0xFF
            descriptor.frontFaceStencil.writeMask = 0x00
        } else {
            fatalError("Invalid stencil mode \(mode)")
        }
        
        return descriptor
    }
}
