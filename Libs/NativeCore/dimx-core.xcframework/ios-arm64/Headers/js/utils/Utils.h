#pragma once
#include <Common.h>
#include <config/Config.h>
#include <quickjspp.hpp>

struct JSValueGuard
{
    JSContext* ctx{ nullptr };
    JSValue val;

    JSValueGuard(JSContext* c, JSValue v) : ctx(c), val(v) {}

    ~JSValueGuard() {
        if (!JS_IsUndefined(val)) {
            JS_FreeValue(ctx, val);
        }
    }
};

template <typename T>
T getJSCallback(JSContext* ctx, JSValue val, const char* name)
{
    JSValueGuard prop{ctx, JS_GetPropertyStr(ctx, val, name)};
    if(!JS_IsUndefined(prop.val)) {
        return qjs::js_traits<T>::unwrap(ctx, prop.val);
    }
    return {};
}

template <typename T>
T JsAnyToFunc(const std::any& any)
{
    if (any.has_value()) {
        const qjs::Value& jsval = std::any_cast<const qjs::Value&>(any);
        if (JS_IsNull(jsval.v) || JS_IsUndefined(jsval.v) || JS_IsUninitialized(jsval.v)) {
            return {};
        }
        return qjs::js_traits<T>::unwrap(jsval.ctx, jsval.v);
    }

    return {};
}

template <typename T>
T JsConfigToFunc(const Config& config)
{
    if (!config.isAny()) {
        return {};
    }

    const qjs::Value& jsval = config.getAny<const qjs::Value&>();
    if (JS_IsNull(jsval.v) || JS_IsUndefined(jsval.v) || JS_IsUninitialized(jsval.v)) {
        return {};
    }
    return qjs::js_traits<T>::unwrap(jsval.ctx, jsval.v);
}

template <typename T>
T JsValToFunc(qjs::Value jsCallback)
{
    if (JS_IsNull(jsCallback.v) || JS_IsUndefined(jsCallback.v) || JS_IsUninitialized(jsCallback.v)) {
        return {};
    }
    return jsCallback.as<T>();
}
