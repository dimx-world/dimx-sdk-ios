import Foundation
import simd
import DimxNative
class FrameContext
{
    var now = 0.0
    var frameDuration = 0.0
    var frameCounter = 0

    var projectionMat = matrix_float4x4()
    var viewMat = matrix_float4x4()
    var viewProjectionMat = matrix_float4x4()
    var cameraMat = matrix_float4x4()
    var cameraPos = vector_float3()

    init() {
    }
    
    func populateFromCore(ptr: UnsafeRawPointer) {
        now = FrameContext_now(ptr)
        frameDuration = FrameContext_frameDuration(ptr)
        FrameContext_viewMat(ptr, &projectionMat)
        FrameContext_projectionMat(ptr, &viewMat)
        FrameContext_viewProjectionMat(ptr, &viewProjectionMat)
        FrameContext_cameraMat(ptr, &cameraMat)
        cameraPos = vector_float3(cameraMat.columns.3[0], cameraMat.columns.3[1], cameraMat.columns.3[2])
        frameCounter = FrameContext_frameCounter(ptr)
    }
}
