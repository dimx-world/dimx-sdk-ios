
uniform bool fUseDepthOcclusion;
uniform sampler2D fDepthMap;
uniform mat3 fDepthMapUVTransform;
uniform float fDepthMapAspectRatio;

float DepthGetMillimeters(in sampler2D depth_texture, in vec2 depth_uv) {
  // Depth is packed into the red and green components of its texture.
  // The texture is a normalized format, storing millimeters.
  vec3 packedDepthAndVisibility = texture(depth_texture, depth_uv).xyz;
  return dot(packedDepthAndVisibility.xy, vec2(255.0, 256.0 * 255.0));
}

// Returns linear interpolation position of value between min and max bounds.
// E.g., DepthInverseLerp(1100, 1000, 2000) returns 0.1.
float DepthInverseLerp(in float value, in float min_bound, in float max_bound) {
  return clamp((value - min_bound) / (max_bound - min_bound), 0.0, 1.0);
}

// Returns a value between 0.0 (not visible) and 1.0 (completely visible)
// Which represents how visible or occluded is the pixel in relation to the
// depth map.
float DepthGetVisibility(in sampler2D depth_texture, in vec2 depth_uv,
                         in float asset_depth_mm) {
  float depth_mm = DepthGetMillimeters(depth_texture, depth_uv);

  // Instead of a hard z-buffer test, allow the asset to fade into the
  // background along a 2 * kDepthTolerancePerMm * asset_depth_mm
  // range centered on the background depth.
  const float kDepthTolerancePerMm = 0.015;
  float visibility_occlusion = clamp(0.5 * (depth_mm - asset_depth_mm) /
    (kDepthTolerancePerMm * asset_depth_mm) + 0.5, 0.0, 1.0);

  // Depth close to zero is most likely invalid, do not use it for occlusions.
  float visibility_depth_near = 1.0 - DepthInverseLerp(
      depth_mm, /*min_depth_mm=*/150.0, /*max_depth_mm=*/200.0);

  // Same for very high depth values.
  float visibility_depth_far = DepthInverseLerp(
      depth_mm, /*min_depth_mm=*/7500.0, /*max_depth_mm=*/8000.0);

  const float kOcclusionAlpha = 0.0;
  float visibility =
      max(max(visibility_occlusion, kOcclusionAlpha),
          max(visibility_depth_near, visibility_depth_far));

  return visibility;
}

float DepthGetBlurredVisibilityAroundUV(in vec2 uv, in float asset_depth_mm)
{
  // Kernel used:
  // 0   4   7   4   0
  // 4   16  26  16  4
  // 7   26  41  26  7
  // 4   16  26  16  4
  // 0   4   7   4   0
  const float kKernelTotalWeights = 269.0;
  float sum = 0.0;

  const float kOcclusionBlurAmount = 0.01;
  vec2 blurriness = vec2(kOcclusionBlurAmount,
                         kOcclusionBlurAmount * fDepthMapAspectRatio);

  float current = 0.0;

  current += DepthGetVisibility(fDepthMap, uv + vec2(-1.0, -2.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(+1.0, -2.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(-1.0, +2.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(+1.0, +2.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(-2.0, +1.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(+2.0, +1.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(-2.0, -1.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(+2.0, -1.0) * blurriness, asset_depth_mm);
  sum += current * 4.0;

  current = 0.0;
  current += DepthGetVisibility(fDepthMap, uv + vec2(-2.0, -0.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(+2.0, +0.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(+0.0, +2.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(-0.0, -2.0) * blurriness, asset_depth_mm);
  sum += current * 7.0;

  current = 0.0;
  current += DepthGetVisibility(fDepthMap, uv + vec2(-1.0, -1.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(+1.0, -1.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(-1.0, +1.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(+1.0, +1.0) * blurriness, asset_depth_mm);
  sum += current * 16.0;

  current = 0.0;
  current += DepthGetVisibility(fDepthMap, uv + vec2(+0.0, +1.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(-0.0, -1.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(-1.0, -0.0) * blurriness, asset_depth_mm);
  current += DepthGetVisibility(fDepthMap, uv + vec2(+1.0, +0.0) * blurriness, asset_depth_mm);
  sum += current * 26.0;

  sum += DepthGetVisibility(fDepthMap, uv , asset_depth_mm) * 41.0;

  return sum / kKernelTotalWeights;
}

float calcOcclusion(vec4 viewPos, vec4 screenSpacePos)
{
    const float kMetersToMillimeters = 1000.0;
    float object_depth_mm = viewPos.z * kMetersToMillimeters * -1.;
    // Computes the texture coordinates to sample from the depth image.
    vec2 depth_uvs = (fDepthMapUVTransform * vec3(screenSpacePos.xy, 1)).xy;
    return DepthGetBlurredVisibilityAroundUV(depth_uvs, object_depth_mm);
}
