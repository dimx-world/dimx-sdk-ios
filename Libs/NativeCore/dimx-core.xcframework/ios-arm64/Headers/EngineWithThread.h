#pragma once

#ifndef DIMX_CORE_ENGINE_WITH_THREAD_H
#define DIMX_CORE_ENGINE_WITH_THREAD_H

#include "Engine.h"
#include <Profiler.h>

#include <atomic>
#include <thread>

class EngineWithThread: public Engine
{
public:
    EngineWithThread(ConfigPtr config)
    : Engine(std::move(config)) {
        mThread = std::thread([this] { threadFunc(); });
    }

    void stop() { mKeepRunning = false; }
    void joinThread() { mThread.join(); };

private:
    void threadFunc() {
        while (mKeepRunning) {
            PROFILER_SCOPE("EngineIteration");
            updateMain();
        }
        // Note: deinitialization must happen on the main thread
        // for correct opengl resources cleanup
        deinitialize();
    }

private:
    std::atomic_bool mKeepRunning{true};
    std::thread mThread;
};

#endif // DIMX_CORE_ENGINE_WITH_THREAD_H