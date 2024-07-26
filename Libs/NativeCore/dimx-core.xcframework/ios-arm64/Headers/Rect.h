#pragma once

#ifndef DIMX_CORE_RECT_H
#define DIMX_CORE_RECT_H

#include "Common.h"

class Rect
{
public:
    Rect(): Rect(0.f, 0.f, 0.f, 0.f) {}
    Rect(float left, float top, float right, float bottom )
    : mLeft(left) , mTop(top) , mRight(right) , mBottom(bottom) {}

    float top()    const { return mTop;           }
    float left()   const { return mLeft;          }
    float bottom() const { return mBottom;        }
    float right()  const { return mRight;         }
    float width()  const { return mRight - mLeft; }
    float height() const { return mBottom - mTop; }

    bool contains(const Vec2& pos) const {
        return (pos.x >= mLeft && pos.x <= mRight && pos.y >= mTop && pos.y <= mBottom);
    }

    Rect& operator+= (const Vec2& v) {
        mLeft += v.x;
        mTop += v.y;
        mRight += v.x;
        mBottom += v.y;
        return *this;
    }

    Rect& operator-= (const Vec2& v) {
        mLeft -= v.x;
        mTop -= v.y;
        mRight -= v.x;
        mBottom -= v.y;
        return *this;
    }

    Rect operator+ (const Vec2& v) const {
        return Rect{mLeft + v.x, mTop + v.y, mRight + v.x, mBottom + v.y};
    }

    Rect operator- (const Vec2& v) const {
        return Rect{mLeft - v.x, mTop - v.y, mRight - v.x, mBottom - v.y};
    }

    Vec2 leftTop() const {
        return Vec2{mLeft, mTop};
    }

    Vec2 rightBottom() const {
        return Vec2{mRight, mBottom};
    }

    std::string toString() const {
        return   std::to_string(mLeft)  + " "
               + std::to_string(mTop)   + " "
               + std::to_string(mRight) + " "
               + std::to_string(mBottom);
    }

    bool empty() const {
        return mLeft == mRight || mTop == mBottom;
    }

    Vec2 center() const {
        return Vec2{(mLeft + mRight) * 0.5f, (mTop + mBottom) * 0.5f};
    }

private:
    float mLeft = 0;
    float mTop = 0;
    float mRight = 0;
    float mBottom = 0;
};

template<>
inline Rect Config::getValue<Rect>() const
{
    float vals[4] = { 0, 0, 0, 0 };
    std::istringstream iss(getValue<String>());

    if (!(iss >> vals[0] >> vals[1] >> vals[2] >> vals[3])) {
        ASSERT(false, "Failed to read Rect from config. str=[" << getValue<String>() << "]");
    }

    return Rect(vals[0], vals[1], vals[2], vals[3]);
}

#endif // DIMX_CORE_RECT_H