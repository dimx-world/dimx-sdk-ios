#pragma once
#include <Common.h>

class Scene;
class NativeScene
{
    NO_COPY_MOVE(NativeScene)
public:
    NativeScene(size_t id, const Scene& coreScene);
    virtual ~NativeScene();

    size_t nativeId() const { return mNativeId; }

private:
    size_t mNativeId{INVALID_INDEX};
};
