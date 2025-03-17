#pragma once
#include <Common.h>
#include <quickjspp.hpp>

class JsEnv;
class Object;
class JsMaterial
{
public:
    JsMaterial(JsEnv* env, Object* obj);

    void setAlpha(double value);
    void setMultColor(double r, double g, double b, double a);
    void setAddColor(double r, double g, double b, double a);

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsMaterial>("JsMaterial")
        .fun<&JsMaterial::setAlpha>("setAlpha")
        .fun<&JsMaterial::setMultColor>("setMultColor")
        .fun<&JsMaterial::setAddColor>("setAddColor");
    }

private:
    JsEnv* mEnv{nullptr};
    Object* mObject{nullptr};
};
