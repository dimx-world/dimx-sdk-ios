// JsContentFactory: manages registration of content templates and creation
// of content instances for JS-exposed objects.
#pragma once
#include <Common.h>
#include "ContentTemplate.h"

#include <quickjspp.hpp>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

class JsEnv;
class JsDimension;
class JsObject;

class JsContentFactory {

    struct ContentRecord {
        std::string clusterPath;
        Config config;
    };

public:
    JsContentFactory(JsEnv* env, JsDimension* dim);

    void addTemplate(const std::string& name, qjs::Value jsConfig, qjs::Value jsCallback);

    void addTemplateExt(const std::string& name,
                        Config config,
                        ContentTemplate::NativeCallback nativeCallback,
                        ContentTemplate::JsCallback jsCallback);

    void addClusterContent(const std::string& clusterPath, qjs::Value jsConfig);
    void addContent(qjs::Value jsConfig, qjs::Value jsCluster);
    
    void onAddDummy(Object* object, qjs::Value jsObject);

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsContentFactory>("JsContentFactory")
            .fun<&JsContentFactory::addTemplate>("addTemplate")
            .fun<&JsContentFactory::addContent>("addContent");
    }

private:
    const ContentTemplate* getTemplate(const std::string& name) const;
    Config expandTemplateConfig(Config config) const;
    void addContentItem(const std::string& clusterPath, Config config);
private:
    JsEnv* mEnv{nullptr};
    JsDimension* mJsDimension{nullptr};

    std::map<std::string, ContentTemplate> mTemplates;
    std::unordered_map<std::string, std::vector<ContentRecord>> mContent;
};
