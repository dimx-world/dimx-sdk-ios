import Foundation
import Metal
import DimxNative

func createIndexTypeMapping() -> [MTLIndexType] {
    var mapping = [MTLIndexType] (repeating: MTLIndexType.uint16, count: IndexType_maxIndex())
    mapping[IndexType_index("UInt16")]  = MTLIndexType.uint16
    mapping[IndexType_index("UInt32")]  = MTLIndexType.uint32
    return mapping
}

let g_indexTypeMapping = createIndexTypeMapping()

func indexTypeFromCore(_ coreType: Int) -> MTLIndexType {
    if coreType >= g_indexTypeMapping.count {
        fatalError("Bad core index ype")
    }
    return g_indexTypeMapping[coreType]
}
