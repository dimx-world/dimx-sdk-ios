
#ifdef DIMX_PLATFORM_ANDROID
#extension GL_OES_EGL_image_external_essl3 : require
#endif

DIMX_FLOAT_PRECISION

in vec2 fTexCoord;
uniform samplerExternalOES fTexture;
out vec4 outColor;

void main()
{
	outColor = texture(fTexture, fTexCoord.xy);
}
