#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsEnv;
class Dimension;
class JsLogger
{
public:
    JsLogger(JsEnv* env, Dimension* dim);

    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsLogger>("JsLogger")
        .fun<&JsLogger::debug>("debug")
        .fun<&JsLogger::info>("info")
        .fun<&JsLogger::info>("log")
        .fun<&JsLogger::warn>("warn")
        .fun<&JsLogger::error>("error");
    }

private:
    JsEnv* mEnv{nullptr};
    Dimension* mDimension{nullptr};
};
