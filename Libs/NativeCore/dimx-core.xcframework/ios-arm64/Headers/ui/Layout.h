#pragma once
#include <Common.h>
#include <config/Config.h>
#include <Rect.h>

DECL_ENUM(HorizontalAlign,  Left,   Center,   Right);
DECL_ESTR(HorizontalAlign, "Left", "Center", "Right");

DECL_ENUM(VerticalAlign,  Top,   Center,   Bottom);
DECL_ESTR(VerticalAlign, "Top", "Center", "Bottom");

namespace ui {

class Layout
{
public:
    void initialize(const Config& config);

    Rect getRegionPx(const Rect& parent) const;

    bool parentBased() const { return mParentBased; }

    void setLeft(float left) { mLeft = left; }
    float left() const { return mLeft; }

    void setTop(float top) { mTop = top; }
    float top() const { return mTop; }

    void setWidth(float width) { mWidth = width; }
    float width() const { return mWidth; }

    void setHeight(float height) { mHeight = height; }
    float height() const { return mHeight; }

    bool horizontalFill() const { return mHorizontalFill; }

    bool verticalFill() const { return mVerticalFill; }

    void setOffsetX(float x) { mOffsetX = x; }
    float offsetX() const { return mOffsetX; }

    float offsetY() const { return mOffsetY; }

    HorizontalAlign hAlign() const { return mHAlign; }

    VerticalAlign vAlign() const { return mVAlign; }

    bool sameLine() const { return mSameLine; }
    float sameLineOffset() const { return mSameLineOffset; }
    float sameLineSpacing() const { return mSameLineSpacing; }

private:
    bool mParentBased{false};

    float mLeft{-1.f};
    float mTop{-1.f};
    float mWidth{-1.f};
    float mHeight{-1.f};

    bool mHorizontalFill{false};
    bool mVerticalFill{false};

    float mOffsetX{0.f};
    float mOffsetY{0.f};

    HorizontalAlign mHAlign{HorizontalAlign::None};
    VerticalAlign mVAlign{VerticalAlign::None};

    bool mSameLine{false};
    float mSameLineOffset{0.f};
    float mSameLineSpacing{-1.f};

    float mRight{std::numeric_limits<float>::quiet_NaN()};
    float mBottom{std::numeric_limits<float>::quiet_NaN()};
};

} // namespace ui
