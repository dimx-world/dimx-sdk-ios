#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsEnv;
class JsLocalStorage
{
public:
    JsLocalStorage(JsEnv* env, ObjectId dimId);

    void set(const std::string& key, qjs::Value jsValue);
    qjs::Value get(const std::string& key);

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsLocalStorage>("JsLocalStorage")
        .fun<&JsLocalStorage::get>("get")
        .fun<&JsLocalStorage::set>("set");
    }

private:
    JsEnv* mEnv{nullptr};
    ObjectId mDimensionId;
};
