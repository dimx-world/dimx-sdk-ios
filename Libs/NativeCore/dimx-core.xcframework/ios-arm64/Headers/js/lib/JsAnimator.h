#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsEnv;
class Animator;
class JsAnimator
{
public:
    JsAnimator(JsEnv* env, Animator* animator);

    void play(const std::string& anim, bool looped);
    void playWithCallback(const std::string& anim, qjs::Value callback);
    void stop();
    void reset();
    void setSpeed(double speed);

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsAnimator>("JsAnimator")
        .fun<&JsAnimator::play>("play")
        .fun<&JsAnimator::playWithCallback>("playWithCallback")
        .fun<&JsAnimator::stop>("stop")
        .fun<&JsAnimator::reset>("reset")
        .fun<&JsAnimator::setSpeed>("setSpeed");
    }

private:
    JsEnv* mEnv{nullptr};
    Animator* mAnimator{nullptr};
    std::function<void()> mCallback;
};
