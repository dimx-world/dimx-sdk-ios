#pragma once
#include <model/ModelBuilder.h>

DECL_ENUM(PlaneAxis,  XY,   XZ)
DECL_ESTR(PlaneAxis, "XY", "XZ")

DECL_ENUM(PlaneOrigin,  Center)
DECL_ESTR(PlaneOrigin, "Center")

class PlaneBuilder: public ModelBuilder
{
public:
    PlaneBuilder(Model& model, const Config& config);

    void serialize(Config& out) override;

    float width() const { return mWidth; }
    float height() const { return mHeight; }

    void setWidth(float width, bool rebuild = true);
    void setHeight(float height, bool rebuild = true);


protected:
    std::shared_ptr<edit::Property> createEditableProperty() override;

private:
    void rebuildNative();

private:
    float mWidth{1.f};
    float mHeight{1.f};
    bool mFlipTexture{false};
    Vec2 mScaleTexture{1.f, 1.f};

    PlaneAxis mPlaneAxis{PlaneAxis::XY};
    PlaneOrigin mOrigin{PlaneOrigin::Center};
};
