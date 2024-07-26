#include <metal_stdlib>
using namespace metal;

typedef struct {
    float2 position [[attribute(0)]];
    float2 texCoord [[attribute(1)]];
} ImageVertex;

typedef struct {
    float4 position [[position]];
    float2 texCoord;
} ImageColorInOut;

vertex ImageColorInOut background_vertex(ImageVertex in [[stage_in]])
{
    ImageColorInOut out;
    out.position = float4(in.position, 0, 1);
    out.texCoord = in.texCoord;
    return out;
}

fragment float4 background_fragment(ImageColorInOut in [[stage_in]],
                                    texture2d<float, access::sample> textureY [[ texture(1) ]],
                                    texture2d<float, access::sample> textureCbCr [[ texture(2) ]])
{
    constexpr sampler colorSampler(mip_filter::linear, mag_filter::linear, min_filter::linear);
    const float4x4 ycbcrToRGBTransform = float4x4(float4(+1.0000f, +1.0000f, +1.0000f, +0.0000f),
                                                  float4(+0.0000f, -0.3441f, +1.7720f, +0.0000f),
                                                  float4(+1.4020f, -0.7141f, +0.0000f, +0.0000f),
                                                  float4(-0.7010f, +0.5291f, -0.8860f, +1.0000f));
    float4 ycbcr = float4(textureY.sample(colorSampler, in.texCoord).r, textureCbCr.sample(colorSampler, in.texCoord).rg, 1.0);
    return ycbcrToRGBTransform * ycbcr;
}
