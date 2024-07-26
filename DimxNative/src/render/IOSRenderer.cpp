#include "IOSRenderer.h"
//#include "IOSEngine.h"
#include "IOSTexture.h"
#include "IOSMaterial.h"
#include "IOSMesh.h"
#include "IOSRenderable.h"
#include <render/RenderLines.h>

struct SwiftRenderer* g_swiftRenderer()
{
    static struct SwiftRenderer callbacks;
    return &callbacks;
}

long Renderer_linesVertsCount(bool debugBuffer)
{
    const RenderLines& lines = debugBuffer ? g_renderer().debugLines() : g_renderer().lines();
    return static_cast<long>(lines.lines().size());
}

const void* Renderer_linesVertsData(bool debugBuffer)
{
    const RenderLines& lines = debugBuffer ? g_renderer().debugLines() : g_renderer().lines();
    return lines.lines().data();
}

long Renderer_linesPolyVertsCount(bool debugBuffer)
{
    const RenderLines& lines = debugBuffer ? g_renderer().debugLines() : g_renderer().lines();
    return static_cast<long>(lines.polygons().size());
}

const void* Renderer_linesPolyVertsData(bool debugBuffer)
{
    const RenderLines& lines = debugBuffer ? g_renderer().debugLines() : g_renderer().lines();
    return lines.polygons().data();
}

void Renderer_drawPoint(bool debugBuffer, const void* posPtr, float worldSize, const void* colPtr)
{
    RenderLines& lines = debugBuffer ? g_renderer().debugLines() : g_renderer().lines();

    Vec3 pos;
    memcpy(&pos, posPtr, sizeof(pos));

    Vec4 col;
    memcpy(&col, colPtr, sizeof(col));

    lines.drawPoint(pos, worldSize, col);
}

void Renderer_drawLine(bool debugBuffer, const void* pos1Ptr, const void* col1Ptr, const void* pos2Ptr, const void* col2Ptr)
{
    RenderLines& lines = debugBuffer ? g_renderer().debugLines() : g_renderer().lines();
    Vec3 pos1;
    memcpy(&pos1, pos1Ptr, sizeof(pos1));

    Vec4 col1;
    memcpy(&col1, col1Ptr, sizeof(col1));
    
    Vec3 pos2;
    memcpy(&pos2, pos2Ptr, sizeof(pos2));

    Vec4 col2;
    memcpy(&col2, col2Ptr, sizeof(col2));

    lines.drawLine(pos1, col1, pos2, col2);
}

void Renderer_drawLineRect(bool debugBuffer, float width, float height, const void* transformPtr, const void* colPtr)
{
    RenderLines& lines = debugBuffer ? g_renderer().debugLines() : g_renderer().lines();

    Mat4 transform;
    memcpy(&transform, transformPtr, sizeof(transform));

    Vec4 col;
    memcpy(&col, colPtr, sizeof(col));

    lines.drawLineRect(width, height, transform, col);
}

void Renderer_drawLineBox(bool debugBuffer, const void* minPtr, const void* maxPtr, const void* transformPtr, const void* colPtr)
{
    RenderLines& lines = debugBuffer ? g_renderer().debugLines() : g_renderer().lines();

    Vec3 min;
    memcpy(&min, minPtr, sizeof(min));

    Vec3 max;
    memcpy(&max, maxPtr, sizeof(max));

    Mat4 transform;
    memcpy(&transform, transformPtr, sizeof(transform));

    Vec4 col;
    memcpy(&col, colPtr, sizeof(col));

    lines.drawLineBox(min, max, transform, col);
}

void Renderer_drawTriangle(bool debugBuffer, const void* pos1Ptr, const void* pos2Ptr, const void* pos3Ptr, const void* colPtr)
{
    RenderLines& lines = debugBuffer ? g_renderer().debugLines() : g_renderer().lines();

    std::vector<Vec3> vec(3);
    memcpy(&vec[0], pos1Ptr, sizeof(Vec3));
    memcpy(&vec[1], pos2Ptr, sizeof(Vec3));
    memcpy(&vec[2], pos3Ptr, sizeof(Vec3));

    Vec4 col;
    memcpy(&col, colPtr, sizeof(col));

    lines.drawPolygon(vec, col);
}

void Renderer_drawPose(bool debugBuffer, const void* transformPtr, float size)
{
    RenderLines& lines = debugBuffer ? g_renderer().debugLines() : g_renderer().lines();

    Mat4 transform;
    memcpy(&transform, transformPtr, sizeof(transform));

    lines.drawPose(transform, size);
}

void IOSRenderer::initialize(const Config& config)
{
    Renderer::initialize(config);

    g_swiftRenderer()->initialize(&config);
}

void IOSRenderer::postInit(const Config& config, CounterPtr counter)
{
    Renderer::postInit(config, counter);
    g_swiftRenderer()->postInit(&config);
}

void IOSRenderer::deinitialize()
{
    Renderer::deinitialize();
}

void IOSRenderer::beginFrame(const FrameContext& frameContext)
{
    Renderer::beginFrame(frameContext);

    beginRenderCamera(0, frameContext); // This needs to go into core ! ! !

    g_swiftRenderer()->beginFrame(&frameContext);
}

void IOSRenderer::endFrame(const FrameContext& frameContext)
{
    Renderer::endFrame(frameContext);
    g_swiftRenderer()->endFrame();
}

void IOSRenderer::getFrameImageData(int width, int height, Buffer& outBuffer)
{
    g_swiftRenderer()->getFrameImageData(width, height, outBuffer.data());
}

std::unique_ptr<NativeTexture> IOSRenderer::createTexture(const Texture& coreTexture)
{
    return std::make_unique<IOSTexture>(coreTexture, this);
}

std::unique_ptr<NativeMaterial> IOSRenderer::createMaterial(const Material& coreMaterial)
{
    return std::make_unique<IOSMaterial>(coreMaterial, this);
}

std::unique_ptr<NativeMesh> IOSRenderer::createMesh(const Mesh& coreMesh)
{
    return std::make_unique<IOSMesh>(coreMesh, this);
}

std::unique_ptr<NativeRenderable> IOSRenderer::createRenderable(const Renderable& coreRenderable)
{
    return std::make_unique<IOSRenderable>(coreRenderable);
}

std::unique_ptr<NativeScene> IOSRenderer::createScene(const Scene& coreScene)
{
    return std::make_unique<NativeScene>(g_swiftRenderer()->createScene(&coreScene), coreScene);
}

void IOSRenderer::deleteScene(size_t id)
{
    return g_swiftRenderer()->deleteScene(id);
}
