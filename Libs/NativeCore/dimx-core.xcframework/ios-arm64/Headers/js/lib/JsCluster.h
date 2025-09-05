#pragma once
#include <Common.h>
#include <quickjspp.hpp>
#include <memory>

class JsEnv;
class JsDimension;
class Config;

struct JsClusterContext {
    JsEnv* env{nullptr};
    JsDimension* dim{nullptr};
    std::string name;
    std::string basePath;
    std::shared_ptr<Config> config;
};

class JsCluster
{
public:
    // Accept raw JS value to avoid uint64_t conversion pitfalls (BigInt vs Number)
    JsCluster(qjs::Value ctxArg);
    const std::string& name() const { return mName; }
    const std::string& basePath() const { return mBasePath; }

    void test(qjs::Value jsConfig);

    static void registerClass(qjs::Context::Module& module) {
        module.class_<JsCluster>("Cluster")
            .constructor<qjs::Value>("Cluster")
            .fun<&JsCluster::name>("name")
            .fun<&JsCluster::basePath>("basePath")
            .fun<&JsCluster::test>("test");
    }

protected:
    std::shared_ptr<Config> getConfig() const { return mConfig; }

private:
    JsEnv* mEnv{nullptr};
    JsDimension* mDimension{nullptr};
    std::string mName;
    std::string mBasePath;
    std::shared_ptr<Config> mConfig;
};
