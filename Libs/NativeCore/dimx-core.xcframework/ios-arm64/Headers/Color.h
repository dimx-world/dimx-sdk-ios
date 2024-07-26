#pragma once

#ifndef DIMX_CORE_COLOR_H
#define DIMX_CORE_COLOR_H

#include "CommonMath.h"

namespace Color {

constexpr Vec4 None   {0.0f, 0.0f, 0.0f, 0.0f};
constexpr Vec4 White  {1.0f, 1.0f, 1.0f, 1.0f};
constexpr Vec4 Red    {1.0f, 0.0f, 0.0f, 1.0f};
constexpr Vec4 Green  {0.0f, 1.0f, 0.0f, 1.0f};
constexpr Vec4 Blue   {0.0f, 0.0f, 1.0f, 1.0f};
constexpr Vec4 Yellow {1.0f, 1.0f, 0.0f, 1.0f};

} // namespace Color

#endif // DIMX_CORE_COLOR_H