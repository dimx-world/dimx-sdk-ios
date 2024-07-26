import Foundation
import Metal
import DimxNative

func createPrimitiveTypeMapping() -> [MTLPrimitiveType] {
    var mapping = [MTLPrimitiveType] (repeating: MTLPrimitiveType.point, count: PrimitiveType_maxIndex())
    mapping[PrimitiveType_index("Points")]  = MTLPrimitiveType.point
    mapping[PrimitiveType_index("Lines")]  = MTLPrimitiveType.line
    mapping[PrimitiveType_index("LineStrip")]  = MTLPrimitiveType.lineStrip
    mapping[PrimitiveType_index("Triangles")]  = MTLPrimitiveType.triangle
    mapping[PrimitiveType_index("TriangleStrip")]  = MTLPrimitiveType.triangleStrip
    return mapping
}

let g_primitiveTypeMapping = createPrimitiveTypeMapping()

func primitiveTypeFromCore(_ coreType: Int) -> MTLPrimitiveType {
    if coreType >= g_primitiveTypeMapping.count {
        fatalError("Bad core primitive ype")
    }
    return g_primitiveTypeMapping[coreType]
}
