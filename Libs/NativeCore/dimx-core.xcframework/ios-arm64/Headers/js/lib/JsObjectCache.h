#pragma once
#include <Common.h>
#include "JsObject.h"
#include <quickjspp.hpp>

class JsEnv;
class JsObjectCache
{
public:
    JsObjectCache(JsEnv* env);

    qjs::Value add(JsLocation* loc, Object* object);
    qjs::Value get(ObjectId id);
    qjs::Value getByName(const std::string& name);
    void remove(ObjectId id);

private:
    JsEnv* mEnv{nullptr};
    std::map<ObjectId, JsObject> mObjects;
    std::map<std::string, ObjectId> mNameToId;
};