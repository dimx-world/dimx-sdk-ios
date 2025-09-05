// NOTE: This header intentionally kept lightweight; it is included from multiple
// translation units. Provide an include guard/pragma and forward declarations
// to avoid accidental redefinitions.
#pragma once

#include <config/Config.h>
#include <quickjspp.hpp>
#include <functional>
#include <string>

class JsContentFactory;
class JsEnv;
class Object; // forward declaration (avoids pulling heavy ECS headers here)

class ContentTemplate {
public:
    using NativeCallback = std::function<void(Object*, const Config&)>;
    using JsCallback = std::function<void(qjs::Value, qjs::Value)>;

    ContentTemplate(const std::string& name,
                    const ContentTemplate* baseTemplate,
                    Config config,
                    NativeCallback nativeCallback,
                    JsCallback jsCallback);

    void createContent(JsEnv* jsEnv, Object* object, qjs::Value jsObject, const std::string& clusterPath, const Config& config) const;

    const Config& config() const { return mConfig; }

private:
    std::string mName;
    const ContentTemplate* mBaseTemplate{nullptr};
    Config mConfig;
    NativeCallback mNativeHandler;
    JsCallback mJsHandler;
};