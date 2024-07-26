DIMX_FLOAT_PRECISION

in vec2 fTexCoord;
in vec4 fViewPosition;
in vec4 fScreenSpacePos;

uniform sampler2D fDepthMap;
uniform mat3 fDepthMapUVTransform;
uniform float fDepthMapAspectRatio;

out vec4 outColor;


const highp float kMaxDepth = 8000.0; // In millimeters.

float DepthGetMillimeters(sampler2D depth_texture, vec2 depth_uv) {
  // Depth is packed into the red and green components of its texture.
  // The texture is a normalized format, storing millimeters.
  vec3 packedDepthAndVisibility = texture(depth_texture, depth_uv).xyz;
  return dot(packedDepthAndVisibility.xy, vec2(255.0, 256.0 * 255.0));
}

// Returns a color corresponding to the depth passed in. Colors range from red
// to green to blue, where red is closest and blue is farthest.
//
// Uses Turbo color mapping:
// https://ai.googleblog.com/2019/08/turbo-improved-rainbow-colormap-for.html
vec3 DepthGetColorVisualization(in float x) {
  const vec4 kRedVec4 = vec4(0.55305649, 3.00913185, -5.46192616, -11.11819092);
  const vec4 kGreenVec4 = vec4(0.16207513, 0.17712472, 15.24091500, -36.50657960);
  const vec4 kBlueVec4 = vec4(-0.05195877, 5.18000081, -30.94853351, 81.96403246);
  const vec2 kRedVec2 = vec2(27.81927491, -14.87899417);
  const vec2 kGreenVec2 = vec2(25.95549545, -5.02738237);
  const vec2 kBlueVec2 = vec2(-86.53476570, 30.23299484);
  const float kInvalidDepthThreshold = 0.01;

  // Adjusts color space via 6 degree poly interpolation to avoid pure red.
  x = clamp(x * 0.9 + 0.03, 0.0, 1.0);
  vec4 v4 = vec4(1.0, x, x * x, x * x * x);
  vec2 v2 = v4.zw * v4.z;
  vec3 polynomial_color = vec3(
    dot(v4, kRedVec4) + dot(v2, kRedVec2),
    dot(v4, kGreenVec4) + dot(v2, kGreenVec2),
    dot(v4, kBlueVec4) + dot(v2, kBlueVec2)
  );

  return step(kInvalidDepthThreshold, x) * polynomial_color;
}

void main()
{
    // remapping uv
//    vec2 uv = vec2(1.0 - fTexCoord.y, 1.0 - fTexCoord.x); // this works
//    vec2 uv = (fDepthMapUVTransform * vec3(fTexCoord.xy, 1)).xy; // doesn't work

    vec2 uv = (fDepthMapUVTransform * vec3(fScreenSpacePos.xy, 1)).xy;

    highp float normalized_depth = DepthGetMillimeters(fDepthMap, uv) / kMaxDepth;
    normalized_depth = clamp(normalized_depth, 0.0, 1.0);
    outColor = vec4(DepthGetColorVisualization(normalized_depth), 1.0);
}
