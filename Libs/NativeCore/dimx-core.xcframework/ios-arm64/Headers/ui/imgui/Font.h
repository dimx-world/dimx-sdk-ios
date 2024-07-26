#pragma once

struct ImFont;

class Font
{
public:
    Font(ImFont* imFont, float scale, int size)
    : mImguiFont(imFont), mScale(scale), mSize(size) {}

    ImFont* imguiFont() const { return mImguiFont; }
    float scale() const { return mScale; }
    int size() const { return mSize; };

private:
    ImFont* mImguiFont{nullptr};
    float mScale{1.f};
    int mSize{0};
};
