//
//  EnumStencilFunction.swift
//  DimxCore
//
//  Created by Sergii Romanov on 08/12/2023.
//  Copyright © 2023 Dimensions. All rights reserved.
//

import Foundation
import Metal
import DimxNative

func createStencilFunctionMapping() -> [MTLCompareFunction] {
    var mapping = [MTLCompareFunction] (repeating: MTLCompareFunction.always, count: StencilFunction_maxIndex() + 1)
    mapping[StencilFunction_index("Never")]  = MTLCompareFunction.never
    mapping[StencilFunction_index("Less")]  = MTLCompareFunction.less
    mapping[StencilFunction_index("LessOrEqual")]  = MTLCompareFunction.lessEqual
    mapping[StencilFunction_index("Greater")]  = MTLCompareFunction.greater
    mapping[StencilFunction_index("GreaterOrEqual")]  = MTLCompareFunction.greaterEqual
    mapping[StencilFunction_index("Equal")]  = MTLCompareFunction.equal
    mapping[StencilFunction_index("NotEqual")]  = MTLCompareFunction.notEqual
    mapping[StencilFunction_index("Always")]  = MTLCompareFunction.always
    mapping[StencilFunction_index("None")]  = MTLCompareFunction.never
    return mapping
}

let g_stencilFunctionMapping = createStencilFunctionMapping()

func stencilFunctionFromCore(_ core: Int) -> MTLCompareFunction {
    if core >= g_stencilFunctionMapping.count {
        fatalError("Bad core texture type")
    }
    
    return g_stencilFunctionMapping[core]
}
