#pragma once
#include <ecs/Component.h>

class Actor;
class ActorManager: public Component
{
    DECLARE_COMPONENT(ActorManager)

public:
    ActorManager(Object* entity, const Config& config);
    ~ActorManager();
    void update(const FrameContext& frameContext);
    ObjectId addActor(const Config& config);
    void clearActors();

private:
    void removeActorInternal(ObjectId id);

private:
    std::vector<std::unique_ptr<Actor>> mActors;

    bool mInsideUpdate{false};
    std::vector<std::function<void()>> mPostUpdateActions;
    std::vector<std::unique_ptr<Actor>> mPendingActors;
};
