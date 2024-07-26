#pragma once

#ifndef DIMX_CORE_LIFE_WATCHER_H
#define DIMX_CORE_LIFE_WATCHER_H

#include "Common.h"

class LifeWatcher
{
public:

    class Watcher
    {
        using WeakT = std::weak_ptr<bool>;
    public:
        Watcher(WeakT wk = {}) : mWeak(std::move(wk)) {}
        Watcher(const Watcher&) = default;
        Watcher(Watcher&& other)
        : mWeak(std::move(other.mWeak))
        {
            other.reset();
        }

        Watcher& operator=(const Watcher&) = default;
        Watcher& operator=(Watcher&& other) {
            mWeak = std::move(other.mWeak);
            other.reset();
            return *this;
        }

        bool alive() const { return !mWeak.expired(); }
        bool empty() const { return !mWeak.owner_before(WeakT{}) && !WeakT{}.owner_before(mWeak);}
        void reset() { mWeak = {}; }
    private:
        WeakT mWeak;
    };

public:
    Watcher lifeWatcher() {
        if (!mShared) {
            mShared = std::make_shared<bool>();
        }
        return Watcher{std::weak_ptr<bool>(mShared)};
    }
private:
    std::shared_ptr<bool> mShared;
};

#endif // DIMX_CORE_LIFE_WATCHER_H