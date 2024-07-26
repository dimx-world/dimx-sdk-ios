#pragma once
#include <Common.h>
#include <DoubleBuffer.h>
#include <mutex>
#include <thread>

class DimensionLoader;
class Loader
{
public:
    // ---- main thread interface ----//
    // NOTE: nothing else should be touched by the main thread!!!
    Loader();

    std::thread::id threadId() { return mThread.get_id(); }
    void pushEvent(Event event);
    void deinitialize();

    // -------------------------------//

    void enterDimension(ObjectId dimId);
    void exitDimension(ObjectId dimId);

    void enterLocation(ObjectId dimId, ObjectId locId);
    void exitLocation(ObjectId dimId, ObjectId locId);

private:
    void threadFunc();

private:
    std::atomic_bool mKeepRunning{true};
    std::thread mThread;

    std::mutex mMutex;
    std::condition_variable mConditionVar;

    DoubleBuffer<Event> mEvents;

    std::map<ObjectId, std::shared_ptr<DimensionLoader>> mDimensions;
};

////////////////////////////////////////////////////////////////////////////////

#define ASSERT_LOADER_THREAD()                                  \
    ASSERT(std::this_thread::get_id() == g_loader().threadId(), \
           "Must be called on the loader thread! "              \
           << "This thread " << std::this_thread::get_id()      \
           << ", loader thread " << g_loader().threadId())
