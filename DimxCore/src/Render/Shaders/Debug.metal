#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    float3 position  [[attribute(0)]];
    float4 color     [[attribute(1)]];
};

struct VertexOut {
    float4 position [[position]];
    float4 color;
};

vertex VertexOut debug_vertex(VertexIn in                    [[stage_in]],
                              constant float4x4 &viewProjMat [[buffer(1)]])
{
    VertexOut out;
    out.position = viewProjMat * float4(in.position, 1);
    out.color = in.color;
    return out;
}

fragment half4 debug_fragment(VertexOut in [[stage_in]])
{
    return half4(in.color);
};
