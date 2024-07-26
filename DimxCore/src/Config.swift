import Foundation
import simd
import DimxNative

func getConfigStr(configPtr: UnsafeRawPointer, key: String) -> String
{
    var strBuf = [Int8](repeating: 0, count: 256)
    if Config_getStr(configPtr, key, &strBuf, UInt32(strBuf.count)) >= 0 {
        return String(cString: strBuf)
    }
    fatalError("Invalid config key [\(key)]")
}

func getConfigVec3(configPtr: UnsafeRawPointer, key: String) -> simd_float3
{
    var tmp = simd_float3()
    Config_getVec3(configPtr, key, &tmp)
    return tmp
}

func getConfigVec4(configPtr: UnsafeRawPointer, key: String) -> simd_float4
{
    var tmp = simd_float4()
    Config_getVec4(configPtr, key, &tmp)
    return tmp
}
