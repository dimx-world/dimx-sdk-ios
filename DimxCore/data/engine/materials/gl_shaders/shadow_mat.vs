DIMX_FLOAT_PRECISION

in vec3 vPosition;

uniform mat4 vViewMat;
uniform mat4 vViewProjMat;
uniform mat4 vModelMat;
uniform mat4 vLightSpaceMat;

out vec4 fViewPosition;
out vec4 fScreenSpacePos;
out vec4 fLightSpacePos;

void main()
{
    float offset = 1.0 / 8192.0;
    vec4 worldSpacePos = vModelMat * vec4(vPosition, 1.0);
    gl_Position = vViewProjMat * worldSpacePos;
    gl_Position.z -= offset * abs(gl_Position.w);

    fViewPosition = vViewMat * worldSpacePos;
    fScreenSpacePos = vec4(gl_Position.xyz / gl_Position.w, 1.0);
    fLightSpacePos = vLightSpaceMat * worldSpacePos;
}
