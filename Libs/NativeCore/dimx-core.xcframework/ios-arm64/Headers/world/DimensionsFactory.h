#pragma once
#include "Dimension.h"

class DimensionsFactory
{
public:
    static DimensionPtr create(const Config& config);
};
