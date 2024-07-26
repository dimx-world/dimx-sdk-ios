#pragma once

#ifndef DIMX_CORE_CAMERA_CTRL_H
#define DIMX_CORE_CAMERA_CTRL_H

#include "CrossObject.h"
#include "FrameContext.h"
#include <config/Config.h>

class CameraCtrl: public CrossObject
{
public:
    CameraCtrl() = default;
    virtual ~CameraCtrl() = default;
    virtual void initialize(const Config& config) {};
    virtual void deinitialize() {};
    virtual void update(const FrameContext& frameContext) {};
};

#endif // DIMX_CORE_CAMERA_CTRL_H