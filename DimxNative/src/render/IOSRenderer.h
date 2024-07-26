#ifndef IOS_RENDERER_INCLUDED_H_INCLUDED
#define IOS_RENDERER_INCLUDED_H_INCLUDED

//---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct SwiftRenderer
{
    void (*initialize)(const void*);
    void (*postInit)(const void*);
    void (*beginFrame)(const void*);
    void (*endFrame)();
    void (*getFrameImageData)(long, long, void*);

    long (*createScene)(const void*);
    void (*deleteScene)(long);
};
struct SwiftRenderer* g_swiftRenderer();

long Renderer_linesVertsCount(bool debugBuffer);
const void* Renderer_linesVertsData(bool debugBuffer);
long Renderer_linesPolyVertsCount(bool debugBuffer);
const void* Renderer_linesPolyVertsData(bool debugBuffer);
void Renderer_drawPoint(bool debugBuffer, const void* posPtr, float worldSize, const void* colPtr);
void Renderer_drawLine(bool debugBuffer, const void* pos1Ptr, const void* col1Ptr, const void* pos2Ptr, const void* col2Ptr);
void Renderer_drawLineRect(bool debugBuffer, float width, float height, const void* transformPtr, const void* colPtr);
void Renderer_drawLineBox(bool debugBuffer, const void* minPtr, const void* maxPtr, const void* transformPtr, const void* colPtr);
void Renderer_drawTriangle(bool debugBuffer, const void* pos1Ptr, const void* pos2Ptr, const void* pos3Ptr, const void* colPtr);
void Renderer_drawPose(bool debugBuffer, const void* transformPtr, float size);

//---- Sample code how to pass swift callback into c++

//typedef void (*TestFunc)(const RendererConfig *);
typedef unsigned long (*TestFunc)(const void*);
void testCallback(TestFunc func);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
//---------------------------------------------------------------

#ifdef __cplusplus
#include <render/Renderer.h>

class IOSRenderer: public Renderer
{
public:
    void initialize(const Config& config) override;
    void postInit(const Config& config, CounterPtr counter) override;
    void deinitialize() override;
    
    void beginFrame(const FrameContext& frameContext) override;
    void endFrame(const FrameContext& frameContext) override;

    void getFrameImageData(int width, int height, Buffer& outBuffer) override;

    std::unique_ptr<NativeTexture> createTexture(const Texture& coreTexture) override;
    std::unique_ptr<NativeMaterial> createMaterial(const Material& coreMaterial) override;
    std::unique_ptr<NativeMesh> createMesh(const Mesh& coreMesh) override;
    std::unique_ptr<NativeRenderable> createRenderable(const Renderable& coreRenderable) override;
    std::unique_ptr<NativeScene> createScene(const Scene& coreScene) override;
    void deleteScene(size_t id) override;
};
#endif // __cplusplus

#endif // IOS_RENDERER_INCLUDED_H_INCLUDED
