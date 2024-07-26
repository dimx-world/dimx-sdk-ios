DIMX_FLOAT_PRECISION

in vec2 fTexCoord;
in vec4 fColor;

uniform sampler2D fTexture;
uniform float fRedChannel;

out vec4 outColor;

void main()
{
    vec4 texCol = texture(fTexture, fTexCoord.st);
    outColor = fColor * (fRedChannel * vec4(1, 1, 1, texCol.r) + (1.0 - fRedChannel) * texCol);
}
