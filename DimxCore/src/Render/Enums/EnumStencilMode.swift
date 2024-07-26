//
//  EnumStencilMode.swift
//  DimxCore
//
//  Created by Sergii Romanov on 08/12/2023.
//  Copyright © 2023 Dimensions. All rights reserved.
//

import Foundation
import DimxNative

enum StencilMode {
    case read, write, none
}

func createStencilModeMapping() -> [StencilMode] {
    var mapping = [StencilMode] (repeating: StencilMode.none, count: StencilMode_maxIndex() + 1)
    mapping[StencilMode_index("Read")]  = StencilMode.read
    mapping[StencilMode_index("Write")]  = StencilMode.write
    mapping[StencilMode_index("None")]  = StencilMode.none
    return mapping
}

let g_stencilModeMapping = createStencilModeMapping()

func stencilModeFromCore(_ core: Int) -> StencilMode {
    if core >= g_stencilModeMapping.count {
        fatalError("Bad core stencil mode")
    }
    
    return g_stencilModeMapping[core]
}
