#include <metal_stdlib>
using namespace metal;

struct VertexUniforms {
    float4x4 projectionMatrix;
};

struct FragmentUniforms {
    float redChannel;
};

struct VertexIn {
    float2 position  [[attribute(0)]];
    float2 texCoords [[attribute(1)]];
    uchar4 color     [[attribute(2)]];
};

struct VertexOut {
    float4 position [[position]];
    float2 texCoords;
    float4 color;
};

vertex VertexOut imgui_vertex(VertexIn in                       [[stage_in]],
                              constant VertexUniforms &uniforms [[buffer(1)]])
{
    VertexOut out;
    out.position = uniforms.projectionMatrix * float4(in.position, 0, 1);
    out.texCoords = in.texCoords;
    out.color = float4(in.color) / float4(255.0);
    return out;
}

fragment half4 imgui_fragment(VertexOut in                            [[stage_in]],
                              constant FragmentUniforms &uniforms     [[buffer(1)]],
                              texture2d<half, access::sample> texture [[texture(0)]])
{
    constexpr sampler linearSampler(coord::normalized, min_filter::linear, mag_filter::linear, mip_filter::linear);
    half4 texColor = texture.sample(linearSampler, in.texCoords);
    return half4(in.color) * (uniforms.redChannel * half4(1, 1, 1, texColor.r) + (1 - uniforms.redChannel) * texColor);
};

