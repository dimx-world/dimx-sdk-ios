uniform vec3 fLightDir;
uniform vec4 fLightVolume;
uniform vec3 fLightAmbientColor;
uniform vec3 fLightDiffuseColor;
uniform vec3 fLightSpecularColor;

vec3 calcLight(vec3 faceNormal)
{
//        vec3 viewDir = normalize(fEyePos - fPosition.xyz);
//        vec3 reflectDir = reflect(fLightDir, faceNormal);  
//        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//        vec3 specular = spec * fLightSpecularColor;  

    float diff = dot(normalize(faceNormal), -fLightDir) * 0.5 + 0.5;
    vec3 lightColor = fLightAmbientColor + fLightDiffuseColor * diff /*+ specular*/;
    return clamp(lightColor, 0.f, 1.0f);
}
