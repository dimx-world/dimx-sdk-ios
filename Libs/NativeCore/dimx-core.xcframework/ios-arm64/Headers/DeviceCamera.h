#pragma once

#ifndef DIMX_CORE_DEVICE_CAMERA_H
#define DIMX_CORE_DEVICE_CAMERA_H

#include "Common.h"
#include <config/Config.h>
#include "FrameContext.h"

DECLARE_PTR(DeviceCamera)
class DeviceCamera
{
public:
    DeviceCamera(){};
    virtual ~DeviceCamera(){};
    virtual void initialize(const Config& config) {};
    virtual void update(const FrameContext& frameContext) {};
};

#endif // DIMX_CORE_DEVICE_CAMERA_H