#pragma once
#include <Common.h>
#include <Input.h>
#include "ImGuiDrawCall.h"
#include <Rect.h>

DECL_ENUM(ImGuiContextType,  ImGui2D,   ImGui3D);
DECL_ESTR(ImGuiContextType, "ImGui2D", "ImGui3D");

struct ImGuiContext;
struct ImFontAtlas;
struct ImPlotContext;
class Texture;
class Material;
class Mesh;
class ResourceManager;


class ImGuiContextWrapper
{
public:
    static constexpr int Button1          = 0; // ImGuiMouseButton_Left;
    static constexpr int Button2          = 1; // ImGuiMouseButton_Right;
    static constexpr int Button3          = 2; // ImGuiMouseButton_Middle;
    static constexpr int ButtonMultitouch = 3;
    static constexpr int ButtonInvalid    = 5; // ImGuiMouseButton_COUNT;

public:
    ImGuiContextWrapper(ImGuiContextType type, ImFontAtlas* fontAtlas);
    ~ImGuiContextWrapper();

    void initialize(ResourceManager& resourceManager, std::function<void()> callback);

    ImGuiContextType type() const { return mType; }

    void processInputEvent(const InputEvent& event);
    void addFocusEvent(bool visible);
    void beginFrame(const FrameContext& frameContext);
    void endFrame();

    uint64_t frameCounter() const { return mFrameCounter; };

    ImGuiContext* native() { return mNative; }

    const Mat4& mvpMat() const { return mMVPMat; }

    const std::vector<ImGuiDrawCall>& drawCalls() const { return mDrawCalls; }
    const std::shared_ptr<Mesh>& mesh() const { return mMesh; }
    const Material* material() const;
    size_t vertexSize() const;
    size_t indexSize() const;
    //------------------------- Screen size handling -------------------------//
    void setScreenSize(const Vec2& size) { mScreenSize = size; }
    const Vec2& screenSize() const { return mScreenSize; }

    const Rect& screenRegionPt() const { return mScreenRegionPt; }
    const Rect& screenRegionPx() const { return mScreenRegionPx; }

    float points2pixels(float points) const { return points * mPoints2PixelsFactor; }
    float pixels2points(float pixels) const { return pixels * mPixels2PointsFactor; }

    Vec2 points2pixelsVec(const Vec2& ptVec) const {return Vec2{ points2pixels(ptVec.x), points2pixels(ptVec.y)}; }
    Vec2 pixels2pointsVec(const Vec2& pxPos) const { return Vec2{pixels2points(pxPos.x), pixels2points(pxPos.y)}; }

    Rect pixels2pointsRect(const Rect& pxRect) const  {
        return Rect{pixels2points(pxRect.left()), pixels2points(pxRect.top()),
                    pixels2points(pxRect.right()), pixels2points(pxRect.bottom())};
    }
    Rect points2pixelsRect(const Rect& ptRect) const {
        return Rect{points2pixels(ptRect.left()), points2pixels(ptRect.top()),
                    points2pixels(ptRect.right()), points2pixels(ptRect.bottom())};
    }
    //------------------------------------------------------------------------//

    bool wantCaptureMouse() const;

    void setRenderTarget(Texture* texture) { mRenderTarget = texture; }
    Texture* renderTarget() const { return mRenderTarget; }

    void onPushContext();
    void onPopContext();

private:
    void updateScreenGeometry();
    void updateRenderData();

private:
    ImGuiContext* mNative{nullptr};
    ImPlotContext* mPlotContext{nullptr};
    ImGuiContextType mType{ImGuiContextType::None};
    std::vector<ImGuiDrawCall> mDrawCalls;
    std::shared_ptr<Mesh> mMesh;

    Vec2 mScreenSize{0, 0};
    float mPoints2PixelsFactor{1.0f};
    float mPixels2PointsFactor{1.0f};
    Rect mScreenRegionPt;
    Rect mScreenRegionPx;

    Mat4 mMVPMat;

    Texture* mRenderTarget{nullptr};
    uint64_t mFrameCounter{0};
};
