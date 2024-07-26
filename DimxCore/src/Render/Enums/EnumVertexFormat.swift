import Foundation
import Metal
import DimxNative

func createVertexFormatMapping() -> [MTLVertexFormat] {
    var mapping = [MTLVertexFormat] (repeating: MTLVertexFormat.invalid, count: ElementType_maxIndex())
    mapping[ElementType_index("Float")]  = MTLVertexFormat.float
    mapping[ElementType_index("Int")]    = MTLVertexFormat.int
    mapping[ElementType_index("Vec2")]   = MTLVertexFormat.float2
    mapping[ElementType_index("Vec2i")]  = MTLVertexFormat.int2
    mapping[ElementType_index("Vec3")]   = MTLVertexFormat.float3
    mapping[ElementType_index("Vec3i")]  = MTLVertexFormat.int3
    mapping[ElementType_index("Vec4")]   = MTLVertexFormat.float4
    mapping[ElementType_index("Vec4i")]  = MTLVertexFormat.int4
    mapping[ElementType_index("Vec4ub")] = MTLVertexFormat.uchar4Normalized
    return mapping
}

let g_vertexFormatMapping = createVertexFormatMapping()

func vertexFormatFromCore(_ coreType: Int) -> MTLVertexFormat {
    if coreType >= g_vertexFormatMapping.count {
        fatalError("Bad core vertex format")
    }
    return g_vertexFormatMapping[coreType]
}
