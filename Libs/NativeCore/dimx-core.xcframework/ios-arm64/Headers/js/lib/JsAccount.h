#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsEnv;
class JsAccount
{
public:
    JsAccount(JsEnv* env);

    const std::string& id() const;
    const std::string& email() const;
    bool valid() const;
    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsAccount>("JsAccount")
        .fun<&JsAccount::id>("id")
        .fun<&JsAccount::email>("email")
        .fun<&JsAccount::valid>("valid");
    }

private:
    JsEnv* mEnv{nullptr};
};
