DIMX_FLOAT_PRECISION

in vec2 vPosition2;
in vec2 vTexCoord;
in vec4 vColor;

uniform mat4 vMVPMat;

out vec2 fTexCoord;
out vec4 fColor;

void main()
{
    fTexCoord = vTexCoord;
    fColor = vColor;
    gl_Position = vMVPMat * vec4(vPosition2, 0, 1);
}
