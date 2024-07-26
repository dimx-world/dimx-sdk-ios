#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsEnv;
class Object;
class ActorManager;
class JsActorManager
{
public:
    JsActorManager(JsEnv* env, ActorManager* actManager);

    std::string add(qjs::Value config);
    void clear();

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsActorManager>("JsActorManager")
        .fun<&JsActorManager::add>("add")
        .fun<&JsActorManager::clear>("clear");
    }

private:
    JsEnv* mEnv{nullptr};
    ActorManager* mActManager{nullptr};
};
