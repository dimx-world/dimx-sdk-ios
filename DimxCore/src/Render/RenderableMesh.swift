//
//  RenderableMesh.swift
//  DimxCore
//
//  Created by Sergii Romanov on 08/12/2023.
//  Copyright © 2023 Dimensions. All rights reserved.
//

import Foundation
import DimxNative

class RenderableMesh
{
    let coreRenderableMesh: UnsafeRawPointer
    unowned let parentRenderable: Renderable
    
    private var scene: Scene?

    let mesh: Mesh
    let material: Material
    
    init(_ parentRend: Renderable, _ coreRendMesh: UnsafeRawPointer) {
        parentRenderable = parentRend
        coreRenderableMesh = coreRendMesh
        
        let coreMeshPtr = RenderableMesh_mesh(coreRenderableMesh)
        mesh = Renderer.instance.meshes[Mesh_nativeId(coreMeshPtr)]!

        let matPtr = RenderableMesh_material(coreRenderableMesh)!
        material = Renderer.instance.materials[Material_nativeId(matPtr)]!
        material.initData(matPtr, mesh, parentRenderable.numSkelJoints())
    }
    
    func getScene() -> Scene {
        return parentRenderable.getScene()
    }
    
    func cameraDistSq() -> Float {
        return RenderableMesh_cameraDistSq(coreRenderableMesh)
    }
    
    func getParentRenderable() -> Renderable {
        return parentRenderable
    }
}
