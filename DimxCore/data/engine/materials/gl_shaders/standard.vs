DIMX_FLOAT_PRECISION
DIMX_USAMPLER2D_PRECISION

in vec3 vPosition;
out vec4 fPosition;

#ifdef VA_vNormal
in  vec3 vNormal;
out vec3 fNormal;
#endif

#ifdef VA_vColor
in vec4 vColor;
#endif

#ifdef VA_vTexCoord
in vec2 vTexCoord;
#endif

#ifdef VA_vJointIndex
in float vJointIndex;
#endif

#ifdef VA_vJointIndices4
in vec4 vJointIndices4;
#endif

#ifdef VA_vJointWeights4
in vec4 vJointWeights4;
#endif

uniform mat4 vViewMat;
uniform mat4 vViewProjMat;
uniform mat4 vModelMat;
uniform mat3 vNormalMat;
uniform mat4 vLightSpaceMat;

#if defined(VA_vJointIndex) || defined(VA_vJointIndices4)
uniform mat4 vJointTransforms[96]; // MAX_JOINTS
#endif

#ifdef VA_vColor
out vec4 fVertColor;
#endif

#ifdef VA_vTexCoord
out vec2 fTexCoord;
#endif

#ifdef MORPH_TARGETS
uniform usampler2D vMorphIndsMap;
uniform sampler2D vMorphVertsMap;

uniform int vNumMeshVerts;
uniform int vMorphVertComps;
uniform int vMorphNumTargets;
uniform int vMorphTargetIds[MAX_MORPH_TARGETS_BLEND];
uniform float vMorphTargetWeights[MAX_MORPH_TARGETS_BLEND];
#endif

out vec4 fWorldPosition;
out vec4 fViewPosition;
out vec4 fScreenSpacePos;
out vec4 fLightSpacePos;

mat4 calcModelMatrix()
{
#ifdef VA_vJointIndex
    return vModelMat * vJointTransforms[int(vJointIndex)];
#elif defined(VA_vJointIndices4)
    mat4 skinMat = vJointTransforms[int(vJointIndices4.x)] * vJointWeights4.x;
    skinMat     += vJointTransforms[int(vJointIndices4.y)] * vJointWeights4.y;
    skinMat     += vJointTransforms[int(vJointIndices4.z)] * vJointWeights4.z;
    skinMat     += vJointTransforms[int(vJointIndices4.w)] * vJointWeights4.w;
    return vModelMat * skinMat;
#else
    return vModelMat;
#endif
}

void main()
{
    vec3 position = vPosition;

#ifdef VA_vNormal
    vec3 normal = vNormal; // add morph
#endif

#ifdef MORPH_TARGETS


    ivec2 indsTexSize = textureSize(vMorphIndsMap, 0);
    ivec2 vertsTexSize = textureSize(vMorphVertsMap, 0);

    for (int i = 0; i < vMorphNumTargets; ++i) {
        int indPos = vMorphTargetIds[i] * vNumMeshVerts + gl_VertexID;
        ivec2 texelPos = ivec2(indPos % indsTexSize.x, indPos / indsTexSize.x);
        int vertLoc = int(texelFetch(vMorphIndsMap, texelPos, 0).r)  * vMorphVertComps;

        if (vertLoc > 0) {
            vec4 morphPos = texelFetch(vMorphVertsMap, ivec2(vertLoc % vertsTexSize.x, vertLoc / vertsTexSize.x), 0);
            position += morphPos.xyz * vMorphTargetWeights[i];
#ifdef MORPH_TARGETS_NORMALS
            vec4 morphNorm = texelFetch(vMorphVertsMap, ivec2(vertLoc+1 % vertsTexSize.x, (vertLoc+1) / vertsTexSize.x), 0);
            normal += morphNorm.xyz * vMorphTargetWeights[i];
#endif
#ifdef MORPH_TARGETS_TANGENTS
            vec4 morphTang = texelFetch(vMorphVertsMap, ivec2(vertLoc+2 % vertsTexSize.x, (vertLoc+2) / vertsTexSize.x), 0);
            //tangent += morphTang.xyz * vMorphTargetWeights[i];
#endif
        }
    }

#endif

    mat4 modelMat = calcModelMatrix();
    fWorldPosition = modelMat * vec4(position, 1.0);
    fPosition = vViewProjMat * fWorldPosition;
    fViewPosition = vViewMat * fWorldPosition;
    fScreenSpacePos = vec4(fPosition.xyz / fPosition.w, 1.0);
    fLightSpacePos = vLightSpaceMat * fWorldPosition;

#ifdef VA_vNormal
    //fNormal = vNormalMat * vNormal;
    fNormal = normalize(transpose(inverse(mat3(modelMat))) * normal);
#endif

#ifdef VA_vColor
    fVertColor = vColor;
#endif

#ifdef VA_vTexCoord
    fTexCoord = vec2(vTexCoord.x, vTexCoord.y);
#endif

    gl_Position = fPosition;
}
