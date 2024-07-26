import Foundation
import Metal
import DimxNative

func createTextureTypeMapping() -> [MTLTextureType] {
    var mapping = [MTLTextureType] (repeating: MTLTextureType.type2D, count: TextureFormat_maxIndex())
    mapping[TextureType_index("Texture2D")]  = MTLTextureType.type2D
    mapping[TextureType_index("TextureCube")]  = MTLTextureType.typeCube
    mapping[TextureType_index("ExternalOES")]  = MTLTextureType.typeTextureBuffer
    return mapping
}

let g_textureTypeMapping = createTextureTypeMapping()

func textureTypeFromCore(_ core: Int) -> MTLTextureType {
    if core >= g_textureTypeMapping.count {
        fatalError("Bad core texture type")
    }
    
    if core == TextureType_index("ExternalOES") {
        fatalError("Unmapped texture type")
    }
    
    return g_textureTypeMapping[core]
}
