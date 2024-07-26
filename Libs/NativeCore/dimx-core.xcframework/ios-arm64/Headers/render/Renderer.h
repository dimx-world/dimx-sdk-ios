#pragma once
#include <CrossObject.h>
#include "RenderLines.h"
#include "NativeTexture.h"
#include "NativeMaterial.h"
#include "NativeMesh.h"
#include "NativeRenderable.h"
#include "NativeScene.h"
#include <Engine.h>
#include <load/Loader.h>
#include <thread>

struct RenderCamera {
    Mat4 projMat;
    Mat4 viewMat;
    Mat4 transformMat;
};

class Texture;
class Material;
class Mesh;
class Renderable;
class Scene;

class Renderer: public CrossObject
{
public:
    Renderer();
    virtual ~Renderer();
    virtual void initialize(const Config& config);
    virtual void postInit(const Config& config, CounterPtr counter);
    virtual void deinitialize();

    virtual void beginFrame(const FrameContext& frameContext);
    virtual void endFrame(const FrameContext& frameContext);

    virtual void beginRenderCamera(size_t idx, const FrameContext& frameContext);
    virtual void endRenderCamera(size_t idx, const FrameContext& frameContext);

    bool shadowsEnabled() const { return mShadowsEnabled; }
    bool backFaceCullEnabled() const { return mBackFaceCullEnabled; }
    FaceVertexOrder frontFaceVertexOrder() const { return mFrontFaceVertexOrder; }
    const Vec4& clearColor() const { return mClearColor; }

    virtual void getFrameImageData(int width, int height, Buffer& outBuffer) {};

    const std::vector<RenderCamera>& renderCameras() const;
    void updateRenderCamera(size_t idx, const Mat4& transfromMat, const Mat4& projMat, const Mat4& viewMat);

    // occlusion map
    bool depthEnabled() const;
    void setDepthEnabled(bool enable);
    const ObjectPtr& depthMap() const;
    const Mat3& depthMapUVTransform() const;
    void setDepthMapUVTransform(const Mat3& mat);
    float depthMapAspectRatio() const;
    void setDepthMapAspectRatio(float ratio);

    RenderLines& debugLines() { return mDebugLines; }
    RenderLines& lines() { return mLines; }

    virtual void deleteScene(size_t id) {};

    template <typename CoreT, typename WeakT, typename Callback>
    void createNative(const CoreT* coreObj, WeakT wk, Callback callback)
    {
        if(std::this_thread::get_id() == g_engine().threadId()) {
            callback(std::move(g_renderer().createNativeObject(*coreObj)));
            return;
        }

        ASSERT(std::this_thread::get_id() == g_loader().threadId(),
               "Unexpected thread for renderer createNative: " << std::this_thread::get_id());

        ASSERT(wk.use_count() > 0, "coreObj not owned by shared_ptr!");
        g_engine().pushEvent([coreObj, wk = std::move(wk), callback = std::move(callback)]{
            if (auto sh = wk.lock()) {
                auto native = g_renderer().createNativeObject(*coreObj);
                g_loader().pushEvent([wk = std::move(wk), rawNative = native.release(), callback = std::move(callback)]{
                    decltype(native) uniqNative(rawNative);
                    if (auto sh = wk.lock()) {
                        callback(std::move(uniqNative));
                    }
                });
            }
        });
    }

protected:
    virtual std::unique_ptr<NativeTexture>    createTexture(const Texture&)       { return {}; }
    virtual std::unique_ptr<NativeMaterial>   createMaterial(const Material&)     { return {}; }
    virtual std::unique_ptr<NativeMesh>       createMesh(const Mesh&)             { return {}; }
    virtual std::unique_ptr<NativeRenderable> createRenderable(const Renderable&) { return {}; }
    virtual std::unique_ptr<NativeScene>      createScene(const Scene&)           { return {}; }

    void setShadowsEnabled(bool enabled) { mShadowsEnabled = enabled; }
    void initRenderCameras(size_t num);

private:
    template <typename CoreT> auto createNativeObject(const CoreT& coreObj);
    auto createNativeObject(const Texture& coreObj)    { return createTexture(coreObj);    }
    auto createNativeObject(const Material& coreObj)   { return createMaterial(coreObj);   }
    auto createNativeObject(const Mesh& coreObj)       { return createMesh(coreObj);       }
    auto createNativeObject(const Renderable& coreObj) { return createRenderable(coreObj); }
    auto createNativeObject(const Scene& coreObj)      { return createScene(coreObj);      }

private:
    bool mShadowsEnabled = true;
    bool mBackFaceCullEnabled = true;
    FaceVertexOrder mFrontFaceVertexOrder = FaceVertexOrder::CounterClockwise;
    Vec4 mClearColor{0, 0, 0, 1};

    std::vector<RenderCamera> mRenderCameras;

    bool mDepthEnabled = false;
    ObjectPtr mDepthMap;
    Mat3 mDepthMapUVTransform; // Identity matrix by default
    float mDepthMapAspectRatio = 1.f;

    RenderLines mLines;
    RenderLines mDebugLines;
};
