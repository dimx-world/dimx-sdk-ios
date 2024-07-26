import Foundation
import Metal
import DimxNative

func createPixelFormatMapping() -> [MTLPixelFormat] {
    var mapping = [MTLPixelFormat] (repeating: MTLPixelFormat.invalid, count: TextureFormat_maxIndex())
    mapping[TextureFormat_index("RGBA")]  = MTLPixelFormat.rgba8Unorm

    mapping[TextureFormat_index("RGB")]  = MTLPixelFormat.invalid

    mapping[TextureFormat_index("RG")]  = MTLPixelFormat.rg8Unorm
    mapping[TextureFormat_index("RG8")]  = MTLPixelFormat.rg8Unorm
    mapping[TextureFormat_index("RED")]  = MTLPixelFormat.r8Unorm
    mapping[TextureFormat_index("DepthComponent")]  = MTLPixelFormat.depth16Unorm
    mapping[TextureFormat_index("DepthComponent32f")]  = MTLPixelFormat.depth32Float
    return mapping
}

let g_pixelFormatMapping = createPixelFormatMapping()

func pixelFormatFromCore(_ core: Int) -> MTLPixelFormat {
    if core >= g_pixelFormatMapping.count {
        fatalError("Bad core texture type")
    }
    
    if g_pixelFormatMapping[core] == MTLPixelFormat.invalid {
        fatalError("Unmapped texture type")
    }
    
    return g_pixelFormatMapping[core]
}
