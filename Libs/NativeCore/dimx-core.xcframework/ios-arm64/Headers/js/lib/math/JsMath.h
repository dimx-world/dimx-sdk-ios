#pragma once
#include <Common.h>
#include <quickjspp.hpp>

#include "JsVec3.h"
#include "JsRay.h"

class JsEnv;
class JsMath
{
public:
    JsMath(JsEnv* env);

    double distance(const JsVec3* a, const JsVec3* b);
    double distanceSq(const JsVec3* a, const JsVec3* b);

    static void registerClass(qjs::Context::Module& module) {
        JsVec3::registerClass(module);
        JsRay::registerClass(module);

        module.class_<JsMath>("JsMath")
        .fun<&JsMath::distance>("distance")
        .fun<&JsMath::distanceSq>("distanceSq");
    }

private:
    JsEnv* mEnv{nullptr};
};