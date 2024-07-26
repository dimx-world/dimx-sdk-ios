#ifndef DimxNative_h
#define DimxNative_h

#ifdef __cplusplus
extern "C" {
#endif

#include "src/ShaderCommon.h"

#include "src/core/EnumsInterface.h"
#include "src/core/ConfigInterface.h"
#include "src/core/TextureInterface.h"
#include "src/core/MeshInterface.h"
#include "src/core/MeshMorphInterface.h"

#include "src/core/RenderableInterface.h"
#include "src/core/RenderableMeshInterface.h"
#include "src/core/MaterialInterface.h"
#include "src/core/VertexAttributeInterface.h"
#include "src/core/ImGuiInterface.h"
#include "src/core/ImGuiContextInterface.h"
#include "src/core/FrameContextInterface.h"

#include "src/core/LightingInterface.h"
#include "src/core/MarkerInterface.h"
#include "src/core/CameraInterface.h"
#include "src/core/VisionInterface.h"
#include "src/core/RaycastInterface.h"
#include "src/core/SettingsInterface.h"
#include "src/core/AnalyticsManagerInterface.h"

#include "src/core/SceneInterface.h"
#include "src/core/SkyboxInterface.h"
#include "src/core/TransformInterface.h"
#include "src/render/IOSTexture.h"
#include "src/render/IOSMaterial.h"
#include "src/render/IOSMesh.h"

#include "src/render/IOSRenderable.h"
#include "src/render/IOSRenderer.h"
#include "src/IOSEngine.h"
#include "src/IOSDisplay.h"
#include "src/IOSDeviceAR.h"
#include "src/IOSCloudAnchorSession.h"
#include "src/IOSInput.h"

#include "src/IOSLogger.h"
#include "src/utils/IString.h"
#include "src/utils/VectorStr.h"
#include "src/utils/MapStr.h"

#ifdef __cplusplus
}
#endif

#endif /* DimxNative_h */
