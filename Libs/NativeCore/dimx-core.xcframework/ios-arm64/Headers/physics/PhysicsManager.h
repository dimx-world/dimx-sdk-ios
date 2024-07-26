#pragma once
#include "CrossObject.h"
#include "PhysicsActor.h"

class Scene;
class PhysicsScene;
class PhysicsManager: public CrossObject
{
public:
    PhysicsManager();
    virtual ~PhysicsManager();

    virtual void initialize(const Config& config);
    virtual void deinitialize();
    virtual void update(const FrameContext& frameContext);

    virtual std::unique_ptr<PhysicsScene> createScene(Scene* coreScene, const Config& config);
    virtual std::unique_ptr<PhysicsActor> createActor(Object* owner, const Config& config);
    
    void enableSimulation(bool enabled);
    bool simulationEnabled() const;
private:
    bool mSimulationEnabled = true;    
};
