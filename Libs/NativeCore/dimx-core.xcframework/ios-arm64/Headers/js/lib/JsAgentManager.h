#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsEnv;
class Object;
class AgentManager;
class JsAgentManager
{
public:
    JsAgentManager(JsEnv* env, AgentManager* agentManager);

    std::string add(qjs::Value config);
    void clear();

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsAgentManager>("AgentManager")
        .fun<&JsAgentManager::add>("add")
        .fun<&JsAgentManager::clear>("clear");
    }

private:
    JsEnv* mEnv{nullptr};
    AgentManager* mAgentManager{nullptr};
};
