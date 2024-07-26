#pragma once
#include <Common.h>
#include "Actor.h"
#include "ActorTrack.h"
#include <config/Config.h>
#include <FrameContext.h>

template <typename T>
class TrackActor: public Actor
{
public:
    TrackActor(Object* obj, const Config& config)
    : Actor(obj, config)
    , mTrack(config.get<const Config&>("track", {}))
    {
        mLoop = config.get("loop", mLoop);
        if (config.contains("duration")) {
            mDuration = config.get<float>("duration");
            if (mDuration <= 0.f) {
                // warn or throw?
                mDuration = mTrack.euclideanLength();
            }
            mSpeed = 1.f / mDuration;
        }
        if (config.contains("speed")) {
            mSpeed = config.get<float>("speed") / mTrack.euclideanLength();
            mDuration = mTrack.euclideanLength() * mSpeed;
        }

        mCurrentPosition = 0.f;
        mCurrentValue = mTrack.evaluate(mCurrentPosition);
    }
    
    bool update(const FrameContext& frameContext) override
    {
        mCurrentPosition += mSpeed * frameContext.frameDuration();

        if (!mLoop && mCurrentPosition > 1.f) {
            mCurrentPosition = 1.f;
        }

        mCurrentValue = mTrack.evaluate(mCurrentPosition);

        return mLoop || mCurrentPosition < 1.f;
    }

    const T& currentValue() const { return mCurrentValue; }
    float currentPosition() const { return mCurrentPosition; }

private:
    ActorTrack<T> mTrack;
    bool mLoop{false};
    float mDuration{0.f};
    float mSpeed{0.f};

    T mCurrentValue{};
    float mCurrentPosition{0};
};
