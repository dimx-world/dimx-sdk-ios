#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsQuat
{
public:
    JsQuat() = default;
    JsQuat(double w, double x, double y, double z) 
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
        .constructor<>()
        .constructor<double, double, double, double>()
        .fun<&JsQuat::setAngles>("setAngles");
    }

public:
    Quat mQuat;
};
