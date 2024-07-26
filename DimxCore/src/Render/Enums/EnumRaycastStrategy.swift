//
//  IOSRaycastStrategy.swift
//  DimxCore
//
//  Created by Sergii Romanov on 15/10/2023.
//  Copyright © 2023 Dimensions. All rights reserved.
//

import Foundation
import ARKit
import DimxNative

@available(iOS 13.0, *)
func createRaycastStrategyMapping() -> [ARRaycastQuery.Target] {
    var mapping = [ARRaycastQuery.Target] (repeating: ARRaycastQuery.Target.estimatedPlane, count: IOSRaycastStrategy_maxIndex())
    mapping[IOSRaycastStrategy_index("ExistingPlaneGeometry")]  = ARRaycastQuery.Target.existingPlaneGeometry
    mapping[IOSRaycastStrategy_index("ExistingPlaneInfinite")]  = ARRaycastQuery.Target.existingPlaneInfinite
    mapping[IOSRaycastStrategy_index("EstimatedPlane")]  = ARRaycastQuery.Target.estimatedPlane
    return mapping
}
let g_raycastStrategyMapping = createRaycastStrategyMapping()
func raycastStrategyFromCore(_ coreType: Int) -> ARRaycastQuery.Target {
    if coreType >= g_raycastStrategyMapping.count {
        fatalError("Bad core raycast strategy")
    }
    return g_raycastStrategyMapping[coreType]
}
