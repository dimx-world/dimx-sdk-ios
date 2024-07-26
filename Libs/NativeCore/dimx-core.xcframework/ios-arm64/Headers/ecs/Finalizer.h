#pragma once
#include <ecs/Component.h>

class Finalizer: public Component
{
    DECLARE_COMPONENT(Finalizer)

    using FinalizeCb = std::function<void()>;
public:
    Finalizer(Object* entity, const Config& config);
    ~Finalizer();

    void subscribe(FinalizeCb cb);

private:
    std::vector<FinalizeCb> mCallbacks;
};
