#pragma once
#include <Common.h>
#include "UICommon.h"

class TextFormatter
{

public:
    struct LineInfo {
        Vec2 offset;
        size_t start{0};
        size_t length{0};
    };

public:
    void reformat(const std::string& text,
                  const Vec2& boxSize,
                  HorizontalAlign alignH,
                  VerticalAlign alignV,
                  float lineHeight);

    const std::vector<LineInfo>& lines() const { return mLines; }

    const Vec2& textSize() const { return mTextSize; }

private:
    size_t findBreak(const char* str, size_t size, char breakChar);
    std::tuple<size_t, float> calcNextLine(const char* str, size_t size, float maxWidth);
    float getTrailSpaceWidth();

private:
    std::string mText;
    Vec2 mBoxSize;
    HorizontalAlign mAlignH;
    VerticalAlign mAlignV;
    float mLineHeight{0};

    std::vector<LineInfo> mLines;
    Vec2 mTextSize;

    float mTrailSpaceWidth{-1.f};
};