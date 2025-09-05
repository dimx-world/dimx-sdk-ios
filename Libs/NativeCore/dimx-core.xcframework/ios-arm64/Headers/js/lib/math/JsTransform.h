#pragma once
#include <Common.h>
#include <Transform.h>
#include <quickjspp.hpp>
#include "JsVec3.h"
#include "JsQuat.h"

class JsTransform
{
public:
    JsTransform() = default;

    JsTransform(JsVec3* pos, JsQuat* rot, JsVec3* scale)
    : mTransform(pos ? pos->core() : Vec3{},
                 rot ? rot->core() : Quat{},
                 scale ? scale->core() : Vec3{1.f, 1.f, 1.f})
    {}

    JsTransform(const Transform& coreTrans)
    : mTransform(coreTrans)
    {}

    const Transform& core() const {
        return mTransform;
    }

    std::shared_ptr<JsVec3> position() const {
        return std::make_shared<JsVec3>(mTransform.position());
    }

    void setPosition(JsVec3* pos) {
        if (pos) {
            mTransform.setPosition(pos->core());
        }
    }

    std::shared_ptr<JsQuat> rotation() const {
        return std::make_shared<JsQuat>(mTransform.rotation());
    }

    void setRotation(JsQuat* rot) {
        if (rot) {
            mTransform.setRotation(rot->core());
        }
    }

    std::shared_ptr<JsVec3> scale() const {
        return std::make_shared<JsVec3>(mTransform.scale());
    }

    void setScale(JsVec3* scale) {
        if (scale) {
            mTransform.setScale(scale->core());
        }
    }

    std::shared_ptr<Transform> transformTrans(JsTransform* trans) const {
        if (!trans) { return nullptr; }
        return std::make_shared<Transform>(mTransform.transformTrans(trans->core()));
    }

    std::shared_ptr<Transform> invTransformTrans(JsTransform* trans) const {
        if (!trans) { return nullptr; }
        return std::make_shared<Transform>(mTransform.invTransformTrans(trans->core()));
    }

    std::shared_ptr<JsVec3> transformPoint(JsVec3* pos) const {
        if (!pos) { return nullptr; }
        return std::make_shared<JsVec3>(mTransform.transformPoint(pos->core()));
    }

    std::shared_ptr<JsVec3> invTransformPoint(JsVec3* pos) const {
        if (!pos) { return nullptr; }
        return std::make_shared<JsVec3>(mTransform.invTransformPoint(pos->core()));
    }

    std::shared_ptr<JsVec3> transformVector(JsVec3* dir) const {
        if (!dir) { return nullptr; }
        return std::make_shared<JsVec3>(mTransform.transformVector(dir->core()));
    }

    std::shared_ptr<JsVec3> invTransformVector(JsVec3* dir) const {
        if (!dir) { return nullptr; }
        return std::make_shared<JsVec3>(mTransform.invTransformVector(dir->core()));
    }

    std::shared_ptr<JsVec3> axisX() const {
        return std::make_shared<JsVec3>(mTransform.axisX());
    }

    std::shared_ptr<JsVec3> axisY() const {
        return std::make_shared<JsVec3>(mTransform.axisY());
    }

    std::shared_ptr<JsVec3> axisZ() const {
        return std::make_shared<JsVec3>(mTransform.axisZ());
    }

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsTransform>("Transform")
        .constructor<>()
        //.constructor<JsVec3*, JsQuat*, JsVec3*>()
        .fun<&JsTransform::position>("position")
        .fun<&JsTransform::setPosition>("setPosition")
        .fun<&JsTransform::rotation>("rotation")
        .fun<&JsTransform::setRotation>("setRotation")
        .fun<&JsTransform::scale>("scale")
        .fun<&JsTransform::setScale>("setScale")
        .fun<&JsTransform::transformTrans>("transformTrans")
        .fun<&JsTransform::invTransformTrans>("invTransformTrans")
        .fun<&JsTransform::transformPoint>("transformPoint")
        .fun<&JsTransform::invTransformPoint>("invTransformPoint")
        .fun<&JsTransform::transformVector>("transformVector")
        .fun<&JsTransform::invTransformVector>("invTransformVector")
        .fun<&JsTransform::axisX>("axisX")
        .fun<&JsTransform::axisY>("axisY")
        .fun<&JsTransform::axisZ>("axisZ");
    }

public:
    Transform mTransform;
};
