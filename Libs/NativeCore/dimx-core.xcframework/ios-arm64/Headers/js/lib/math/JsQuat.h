#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsQuat
{
public:
    JsQuat(double w = 1.0, double x = 0.0, double y = 0.0, double z = 0.0) 
    : mQuat(w, x, y, z)
    {}

    JsQuat(const Quat& quat)
    : mQuat(quat)
    {}

    const Quat& core() const {
        return mQuat;
    }

    void setAngles(double x, double y, double z) {
        mQuat = angles2quat(Vec3{x, y, z});
    }

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsQuat>("Quat")
        .constructor<double, double, double, double>()
        .fun<&JsQuat::setAngles>("setAngles");
    }

public:
    Quat mQuat;
};
