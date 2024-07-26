#pragma once

#ifndef DIMX_CORE_CROSS_OBJECT_H
#define DIMX_CORE_CROSS_OBJECT_H

#include <Common.h>

class CrossObject
{
    NO_COPY_MOVE(CrossObject);
public:
    CrossObject() = default;
    virtual ~CrossObject() = default;
};

#endif // DIMX_CORE_CROSS_OBJECT_H