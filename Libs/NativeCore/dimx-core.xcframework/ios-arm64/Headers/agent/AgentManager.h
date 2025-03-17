#pragma once
#include <ecs/Component.h>

class Agent;
class AgentManager: public Component
{
    DECLARE_COMPONENT(AgentManager)

public:
    AgentManager(Object* entity, const Config& config);
    ~AgentManager();
    void update(const FrameContext& frameContext);
    ObjectId addAgent(const Config& config);
    void clearAgents();

private:
    void removeAgentInternal(ObjectId id);

private:
    std::vector<std::unique_ptr<Agent>> mAgents;

    bool mInsideUpdate{false};
    std::vector<std::function<void()>> mPostUpdateActions;
    std::vector<std::unique_ptr<Agent>> mPendingAgents;
};
