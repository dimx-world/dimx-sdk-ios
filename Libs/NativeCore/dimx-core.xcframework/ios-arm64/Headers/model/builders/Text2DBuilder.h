#pragma once
#include <model/ModelBuilder.h>

class Mesh;
class Text2DBuilder: public ModelBuilder
{
public:
    Text2DBuilder(Model& model, const Config& config);

    void serialize(Config& out) override;
    void setText(const std::string& text, bool rebuild = true);
    void setFontSize(float size, bool rebuild = true);
    void setFramePadding(float value, bool rebuild = true);
    void setFrameWidth(float value, bool rebuild = true);
    void setFrameHeight(float value, bool rebuild = true);
    void setBorderSize(float value, bool rebuild = true);
    void setCornerRadius(float value, bool rebuild = true);
    void setTextColor(const Vec4& value, bool rebuild = true);
    void setBackgroundColor(const Vec4& value, bool rebuild = true);
    void setBorderColor(const Vec4& value, bool rebuild = true);
    void setFlatMaterial(bool value, bool rebuild = true);

protected:
    std::shared_ptr<edit::Property> createEditableProperty() override;

private:
    void rebuildNative();
    AABB getFrameAABB(const AABB& textAabb);
    void buildTextMesh(Mesh& mesh, float posZ);
    void buildFrameMesh(Mesh& mesh, const AABB& aabb, float posZ);
    void buildBorderMesh(Mesh& mesh, const AABB& aabb, float posZ);

private:
    std::string mText;
    float mFontSize{10.f}; // Measured in cm

    float mFramePadding{0.f};
    float mFrameWidth{0.f};
    float mFrameHeight{0.f};
    float mBorderSize{0.f};
    float mCornerRadius{0.f};
    size_t mNumCornerSegments{4};

    Vec4 mTextColor{1.f, 1.f, 1.f, 1.f};
    Vec4 mBackgroundColor{0.f, 0.f, 0.f, 0.f};
    Vec4 mBorderColor{1.f, 1.f, 1.f, 1.f};

    bool mFlatMaterial{false};
};
