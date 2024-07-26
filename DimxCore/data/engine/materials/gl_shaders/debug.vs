DIMX_FLOAT_PRECISION

in vec3 vPosition;
in vec4 vColor;

uniform mat4 vMVPMat;

out vec4 fColor;

void main()
{
    fColor = vColor;
    gl_Position = vMVPMat * vec4(vPosition, 1);
}
