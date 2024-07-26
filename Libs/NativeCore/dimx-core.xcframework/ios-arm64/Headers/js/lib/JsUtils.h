#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsEnv;
class Dimension;
class JsUtils
{
public:
    JsUtils(JsEnv* env, Dimension* dim);

    void openUrl(const std::string& url);

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsUtils>("JsUtils")
        .fun<&JsUtils::openUrl>("openUrl");
    }

private:
    JsEnv* mEnv{nullptr};
    Dimension* mDimension{nullptr};
};
