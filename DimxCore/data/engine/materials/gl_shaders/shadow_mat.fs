DIMX_FLOAT_PRECISION

in vec4 fViewPosition;
in vec4 fScreenSpacePos;
in vec4 fLightSpacePos;

#include "shadow_map.inc"

#ifdef USE_DEPTH_FOR_OCCLUSION
#include "depth_occlusion.inc"
#endif

out vec4 outColor;

void main()
{
    float shadow = calcShadow(fLightSpacePos);
    if (shadow < 0.01) {
        discard;
    }
    outColor = vec4(0.0, 0.0, 0.0, shadow);

#ifdef USE_DEPTH_FOR_OCCLUSION
    if (fUseDepthOcclusion) {
        outColor *= calcOcclusion(fViewPosition, fScreenSpacePos);
    }
#endif

}
